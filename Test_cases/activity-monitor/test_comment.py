from time import sleep, time
from datetime import datetime
from sys import argv
import re
import csv

from bs4 import BeautifulSoup
import requests

from selenium import webdriver
from selenium.common.exceptions import NoSuchElementException, StaleElementReferenceException
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.by import By



def init_chromedriver():
    """
    Initializes the chromedriver to not to load images
    :return: A chromedriver object
    """
    chrome_options = webdriver.ChromeOptions()
    prefs = {"profile.managed_default_content_settings.images": 2, "profile.default_content_settings.state.flash": 0}
    chrome_options.add_experimental_option("prefs", prefs)
    return webdriver.Chrome('./chromedriver', chrome_options=chrome_options)

username=" "
password=" "

driver = init_chromedriver()
driver.get("https://www.wordpress.com/log-in")
el1 = '#usernameOrEmail'
element = driver.find_element_by_css_selector(el1)
element.send_keys(username)
el2 = '#password'
element2 = driver.find_element_by_css_selector(el2)
element2.send_keys(password)

elem = driver.find_element_by_css_selector(".button")
elem.click()
sleep(2)

driver.get(" ")
count = 0

while(True):
        try :
            #driver.execute_script("return arguments[0].scrollIntoView();", comsel)
            count += 1
            driver.find_element_by_class_name('comments__form').click()
            comsel = driver.find_element_by_css_selector('.focused > textarea:nth-child(2)')

            comsel.send_keys("Sure!")
            send = driver.find_element_by_class_name('is-visible')
            send.click()
            print ("posted here")
            break
        except (StaleElementReferenceException, NoSuchElementException):
            driver.execute_script("window.scrollBy(0, 200);")
            sleep (0.5)
            if count > 100:
                break