import sys
import os
import re
import time
import json
import csv
import glob

import arrow

from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


def enable_download_headless(driver, download_path):
    """
    The download to a specified folder works thanks to this post
    https://medium.com/@moungpeter/how-to-automate-downloading-files-using-python-selenium-and-headless-chrome-9014f0cdd196
    """
    driver.command_executor._commands['send_command'] = ('POST', '/session/$sessionId/chromium/send_command')
    params = {'cmd':'Page.setDownloadBehavior', 'params': {'behavior': 'allow', 'downloadPath': download_path}}
    driver.execute('send_command', params)


def get_web_driver(binary_location, driver_type, download_path, headless):
    """ Initialize and configure the web driver """
    if driver_type != 'chrome':
        raise ValueError(f'{driver_type} not supported. Use chrome or update code')

    chrome_options = webdriver.ChromeOptions()
    chrome_options.binary_location = binary_location
    if headless:
        chrome_options.add_argument('--headless')
    chrome_options.add_argument('--disable-notifications')
    chrome_options.add_experimental_option('prefs', {
        'download.default_directory': download_path,
        'download.prompt_for_download': False,
        'download.directory_upgrade': True,
        'safebrowsing.enabled': True
        }
    )

    return webdriver.Chrome(options=chrome_options)


def sign_in(driver, username, password):
    """ Sign in to wordpress.com """

    # Calling /stats should redirect to login
    url = 'https://wordpress.com/stats'
    driver.get(url)

    # fill in and submit username
    email_txt = driver.find_element_by_css_selector('#usernameOrEmail')
    email_txt.send_keys(username)

    submit_btn = driver.find_element_by_class_name('button')
    submit_btn.click()

    time.sleep(3)

    # fill in and submit password
    pass_txt = driver.find_element_by_css_selector('#password')
    pass_txt.send_keys(password)

    submit_btn = driver.find_element_by_class_name('button')
    submit_btn.click()


def assemble_url(domain, date, stat_page=None):
    """
    Helper function to build the correct url for corresponding date and stat page
    """
    base_url = 'https://wordpress.com/stats/day/'
    date_str = date.strftime('%Y-%m-%d')

    if stat_page is not None:
        base_url = f'{base_url}{stat_page}/'

    return f'{base_url}{domain}?startDate={date_str}'


def download_csv_files(driver, domain, from_date, to_date, stat_pages):
    """
    Download CSV files from respective pages
    """
    date = arrow.get(from_date).shift(days=-1)  # start a day early so incrementing
                                                # can be kept next to loop condition
    to_date = arrow.get(to_date)

    while date < to_date:
        date = date.shift(days=1)
        print(f'Downloading csv for {date}')
        for stat_page in stat_pages:
            try:
                url = assemble_url(domain, date, stat_page)
                driver.get(url)
                time.sleep(1)
                driver.execute_script('window.scrollTo(0, document.body.scrollHeight);')
                CSV_DOWNLOAD_BUTTON_CLASS_NAME = 'stats-download-csv'

                wait = 5 # seconds
                download_csv_btn = WebDriverWait(driver, wait).until(
                    EC.presence_of_element_located(
                        (By.CLASS_NAME, CSV_DOWNLOAD_BUTTON_CLASS_NAME)))
                #button = driver.find_element_by_class_name(CSV_DOWNLOAD_BUTTON_CLASS_NAME)
                if download_csv_btn.is_enabled():
                    download_csv_btn.click()
                    # Don't hurry, don't spam, don't get blocked by wordpress ;)
                    time.sleep(0.5)
                else:
                    print(f'No CSV data for {stat_page} for {date}')
            except Exception as e:
                print(f'Error downloading of {stat_page} for {date}: {e}')
                raise e
            # Humans are slow, machines are fast, tell machine to be slow
            time.sleep(0.5)


def extract_visitor_numbers(driver, domain, from_date, to_date):
    """
    Extract visitor numbers from HTML of 'Statistics and Insights' graph
    Mainly for the uniqueue user count that we can't get from the CSV files
    """

    date = arrow.get(from_date).shift(days=-1)  # start a day early so incrementing
                                                # can be kept next to loop condition
    to_date = arrow.get(to_date)
    output = []
    while date < to_date:
        date = date.shift(days=1)
        date_str = date.strftime('%Y-%m-%d')
        print(f'Extracting visitor count for {date_str}')

        # TODO: it seems like there could be a more efficient way of
        #       extracting the data by "clicking" on the interactive diagram
        #       since neighboring days to the one that was loaded are also
        #       shown. That would reduce the number of page loads
        #       by a factor of 10-20
        # Load page for the corresponding day
        driver.get(assemble_url(domain, date))
        #time.sleep(1)

        try:
            WAIT = 5 # seconds
            STATS_EL_CLASS_NAME = 'stats-tab'
            WebDriverWait(driver, WAIT).until(
                EC.presence_of_element_located(
                    (By.CLASS_NAME, STATS_EL_CLASS_NAME)))
            stats = driver.find_elements_by_class_name(STATS_EL_CLASS_NAME)

            # For each stat-tab get the text of the 'value' element
            vals = [stat.find_element_by_class_name('value').text for stat in stats ]
            # The values are in order [views, visitors, likes, comments]
            # The names/labels are language specific so we have to rely on the order
            output.append({
                'date': date_str,
                'views': vals[0],
                'visitors': vals[1],
                'likes': vals[2],
                'comments': vals[3]
            })

        except TimeoutException:
            print('Error "stats-tab" not found. ')
        except Exception as e:
            print(f'Error, unexpected: {e}')

        # be slow, respect services that are provided for free
        time.sleep(1)

    return output


def merge_csv_files(domain, download_path, stat_pages, from_date, to_date):
    """
    After the download there are a bunch of csv files, one per date and per stat page.
    Group by stat page
    Will create a "table" where each line corresponds to a date
    and the keys from individual files will be columns.
    N.B some columns are aggregate of others, watch out not to count your clicks twice
        when analyzing the results ;)
    """

    from_date_str = arrow.get(from_date).strftime('%Y_%m_%d')
    to_date_str = arrow.get(to_date).strftime('%Y_%m_%d')

    for stat_page in stat_pages:
        merged_stats = [] # will contain an entry for each date/file
        all_keys = {} # will contain the set of all keys/columns across all dates/files
                      # use a dict as a kind of ordered set; don't expect a lot of overhead
        all_keys['date'] = None # that will be the first column in the aggregated file

        glob_str = os.path.join(download_path, domain) + f'_{stat_page}*'
        # iterate over all files for the stat_page
        for fname in glob.glob(glob_str):
            # parse date from filename
            date = fname[-14:-4].split('_')
            # N.B. Not sure how the date format in the downloaded filename
            # is determined (client or server) but it looks like it is the shitty
            # US format as of the last time I ran the script ...
            date = f'{date[2]}-{date[0]}-{date[1]}'

            # open file and read out values
            # we expect two columns per row
            # first is an key of some sort depending on the stat_page
            #     (e.g. 'Austria' for the country views)
            # the second is the count
            # in the output we want the keys to be columns
            entry = {'date': date}
            with open(fname, encoding="utf-8-sig") as fin:
                for row in csv.reader(fin, skipinitialspace=True):
                    key, val = row[0], row[1]
                    all_keys[key] = None
                    if key not in entry:
                        entry[key] = val
                    else:
                        # Turns out that the downloaded csv does not contain
                        # all the information and as a result there are duplicate entries
                        # for example https://google.com and google.com on wordpress.com/stats
                        # are considered different but in the csv file they both appear as
                        # simply google.com
                        # The values of those columns need to be summed up
                        entry[key] += val

            merged_stats.append(entry)

            fout_name = os.path.join(download_path, f'_{domain}_{stat_page}_{from_date}_{to_date}.csv')
            with open(fout_name, 'w') as fout:
                csv_writer = csv.DictWriter(fout, list(all_keys))
                csv_writer.writeheader()
                csv_writer.writerows(merged_stats)



if __name__ == '__main__':

    if len(sys.argv) != 2:
        print(f'Usage: python {sys.argv[0]} conf.json')
        sys.exit()

    with open(sys.argv[1]) as f:
        args = json.load(f)

    with get_web_driver(args['browser'], args['webdriver'],
                    args['out-path'], args['headless']) as driver:

        enable_download_headless(driver, args['out-path'])
        sign_in(driver, username=args['username'], password=args['password'])
        time.sleep(3)

        download_csv_files(
            driver, args['domain'], args['from-date'], args['to-date'], args['stat-pages'])
        # merge data and write it to files by stat-page/category
        merge_csv_files(args['domain'], args['out-path'], args['stat-pages'], args['from-date'], args['to-date'])

        # get visitor stats
        stats = extract_visitor_numbers(
            driver, args['domain'], args['from-date'], args['to-date'])
        # write stats to file
        fout_name = f'_{args["domain"]}_visitors_{args["from-date"]}_{args["to-date"]}.csv'
        with open(os.path.join(args['out-path'], fout_name), 'w') as fout:
            csv_writer = csv.DictWriter(fout, ['date', 'views', 'visitors', 'likes', 'comments'])
            csv_writer.writeheader()
            csv_writer.writerows(stats)