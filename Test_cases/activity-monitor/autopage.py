from selenium import webdriver
from selenium.webdriver.common import keys
from selenium.common.exceptions import NoSuchElementException
from webdriver_manager.chrome import ChromeDriverManager

import unittest
import time
import pickle
from pathlib import Path


class WordpressAutomation(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Chrome(ChromeDriverManager().install())
        self.driver.get('URL')

    def login(self):
        login = self.driver.find_element_by_id('user_login')
        login.send_keys('USERNAME')
        password = self.driver.find_element_by_id('user_pass')
        password.send_keys('PASSWORD')
        self.driver.find_element_by_id('wp-submit').click()
        time.sleep(5)
        pickle.dump(self.driver.get_cookies(), open('cookies.pkl', 'wb'))

    def test_first(self):
        cookies = Path('cookies.pkl')
        if not cookies.is_file():
            self.login()
        else:
            cookies = pickle.load(open('cookies.pkl','rb'))
            for cookie in cookies:
                self.driver.add_cookie(cookie)
        with open('test.txt','r') as file:
            for page in file.readlines():
                self.driver.get('URL/post-new.php?post_type=page')
                xpath = '//input[@name="post_title"]'
                self.driver.find_element_by_xpath(xpath).send_keys(page)
                self.driver.find_element_by_id('publish').click()
                time.sleep(5)

    def tearDown(self):
        time.sleep(10)
        self.driver.close()

if __name__ == '__main__':
    unittest.main()