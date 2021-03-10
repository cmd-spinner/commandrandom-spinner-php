from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
import unittest, time, re



class Testy(unittest.TestCase):
	def setUp(self):
		self.driver = webdriver.Remote(command_executor='http://pcsmrcka.fit.vutbr.cz:4444/wd/hub', desired_capabilities=DesiredCapabilities.FIREFOX)
		self.driver.implicitly_wait(30)
		self.base_url = "http://pcsmrcka.fit.vutbr.cz:8123/"
		self.verificationErrors = []
		self.accept_next_alert = True
	
	# pociatocna stranka
	def getHTTP(self, driver, user):
		driver.get(self.base_url + "/"+user+"/")	
		self.assertEqual("WordPress "+user+" | Just another WordPress site", driver.title)
	
	# prihlasenie administratora
	def login_admin(self, driver, user, password):
		driver.find_element_by_link_text("Log in").click()
		driver.find_element_by_id("user_login").clear()
		driver.find_element_by_id("user_login").send_keys(user)
		self.assertEqual(driver.find_element_by_id("user_login").get_attribute("value"), user)
		driver.find_element_by_id("user_pass").clear()
		driver.find_element_by_id("user_pass").send_keys(password)
		self.assertEqual(driver.find_element_by_id("user_pass").get_attribute("value"), password)
		driver.find_element_by_id("wp-submit").click()
		
	# odhlasenie administratora
	def logout_admin(self, driver):
		driver.find_element_by_link_text("Log out").click()
		
	# instalacia pluginu
	def install_and_activate_plugin(self, driver, user, name):			
		driver.find_element_by_xpath("//li[@id='menu-plugins']/a").click()		
		data1 = u'\u2039'
		data2 = u'\u2014'	
		self.assertEqual("Plugins "+data1+" Wordpress "+user+" "+data2+" WordPress", driver.title)
		driver.find_element_by_xpath("//div[@id='wpbody-content']/div[3]/h2/a").click()
		self.assertEqual("Add Plugins "+data1+" Wordpress "+user+" "+data2+" WordPress", driver.title)
		# zada do vyhladavania
		driver.find_element_by_xpath("//input[@name='s']").clear()
		driver.find_element_by_xpath("//input[@name='s']").send_keys(name)
		driver.find_element_by_xpath("//input[@id='search-submit']").click()
		self.assertEqual(driver.find_element_by_xpath("//input[@name='s']").get_attribute("value"), name)	
		# nainstaluje plugin	
		driver.find_element_by_xpath("//a[contains(text(),'Groups')]").click()
		driver.find_element_by_xpath("//a[contains(text(),'Install Now')]").click()
		driver.find_element_by_xpath("//a[contains(text(),'Activate Plugin')]").click()


	# odinstalacia pluginu
	def deactivate_and_uninstall_plugin(self, driver, user, name):
		driver.find_element_by_xpath("//li[@id='menu-plugins']/a").click()
		data1 = u'\u2039'
		data2 = u'\u2014'	
		self.assertEqual("Plugins "+data1+" Wordpress "+user+" "+data2+" WordPress", driver.title)
		# deaktivacia
		driver.find_element_by_xpath("//tr[@id='groups']/td/div/span/a").click()
		# odinstalovanie
		driver.find_element_by_css_selector("#groups > td.plugin-title > div.row-actions.visible > span.delete > a.delete").click()
		driver.find_element_by_id("submit").click()


	# vytvorenie skupiny
	def add_group(self, driver, name):
		driver.find_element_by_xpath("//img[contains(@src,'http://pcsmrcka.fit.vutbr.cz:8123/xtichy23/wp-content/plugins/groups//images/groups.png')]").click()
		# pridanie
		driver.find_element_by_xpath("//div[@id='wpbody-content']/div[3]/div[2]/a").click()	
		driver.find_element_by_id("name-field").clear()
		driver.find_element_by_id("name-field").send_keys(name)
		self.assertEqual(driver.find_element_by_xpath("//form[@id='add-group']/div/div/input").get_attribute("value"), name)
		driver.find_element_by_css_selector("input.button.button-primary").click()
		# pridalo sa do tabulky
		self.assertEqual(driver.find_element_by_xpath("//form[@id='groups-action']/table/tbody/tr[3]/td[2]").text, name)

	# uprava skupiny
	def edit_group(self, driver, newname):
		driver.find_element_by_xpath("//img[contains(@src,'http://pcsmrcka.fit.vutbr.cz:8123/xtichy23/wp-content/plugins/groups//images/groups.png')]").click()
		# uprava
		driver.find_element_by_xpath("//form[@id='groups-action']/table/tbody/tr[3]/td[5]/a/img").click()
		driver.find_element_by_id("name-field").clear()
		driver.find_element_by_id("name-field").send_keys(newname)
		driver.find_element_by_css_selector("input.button.button-primary").click()
		self.assertEqual(driver.find_element_by_xpath("//form[@id='groups-action']/table/tbody/tr[3]/td[2]").text, newname)
 
	# vymazanie skupiny
	def delete_group(self, driver, name):
		driver.find_element_by_xpath("//img[contains(@src,'http://pcsmrcka.fit.vutbr.cz:8123/xtichy23/wp-content/plugins/groups//images/groups.png')]").click()
		self.assertEqual(driver.find_element_by_xpath("//form[@id='groups-action']/table/tbody/tr[3]/td[2]").text, name)
		# vymazanie
		driver.find_element_by_xpath("//form[@id='groups-action']/table/tbody/tr[3]/td[6]/a/img").click()
		driver.find_element_by_css_selector("input.button.button-primary").click()


    #TEST prihlasenia a odhlasenia
	def test_1_login_logout(self):
		driver = self.driver 
		self.getHTTP(driver, "xtichy23")       
		self.login_admin(driver, "xtichy23","bfas")       
		self.getHTTP(driver, "xtichy23") 
		self.logout_admin(driver)
       
    #TEST instalacie a aktivacie pluginu
	def test_2_install_and_uninstall_plugin(self):
		driver = self.driver 
		self.getHTTP(driver, "xtichy23")       
		self.login_admin(driver, "xtichy23","bfas")   
		self.install_and_activate_plugin(driver, "xtichy23", "groups")
		self.deactivate_and_uninstall_plugin(driver, "xtichy23", "groups")
	
	
	#TEST vytvorenie skupiny
	def test_3_add_group(self):
		driver = self.driver 
		self.getHTTP(driver, "xtichy23")       
		self.login_admin(driver, "xtichy23","bfas")  
		self.install_and_activate_plugin(driver, "xtichy23", "groups")
		self.add_group(driver, "VIP")
		
	#TEST uprava skupiny
	def test_4_edit_group(self):
		driver = self.driver 
		self.getHTTP(driver, "xtichy23")       
		self.login_admin(driver, "xtichy23","bfas")  
		self.edit_group(driver, "VIP members")
		
	#TEST vymazanie skupiny
	def test_5_delete_group(self):
		driver = self.driver 
		self.getHTTP(driver, "xtichy23")       
		self.login_admin(driver, "xtichy23","bfas")  
		self.delete_group(driver, "VIP members")
		
	def is_element_present(self, how, what):
		try: self.driver.find_element(by=how, value=what)
		except NoSuchElementException, e: return False
		return True
    
	def is_alert_present(self):
		try: self.driver.switch_to_alert()
		except NoAlertPresentException, e: return False
		return True
    
	def close_alert_and_get_its_text(self):
		try:
			alert = self.driver.switch_to_alert()
			alert_text = alert.text
			if self.accept_next_alert:
				alert.accept()
			else:
				alert.dismiss()
			return alert_text
		finally: self.accept_next_alert = True
    
	def tearDown(self):
		self.driver.quit()
		self.assertEqual([], self.verificationErrors)

if __name__ == "__main__":
    unittest.main()