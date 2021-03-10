import selenium.webdriver as webdriver
import selenium.webdriver.support.ui as ui
from selenium.webdriver.common.keys import Keys
from time import sleep
import json

with open("softwar_details.json") as file:
 	details = json.load(file)

cat_data = {
	"Photos and Images"  		: "in-group-15",
	"System Tuning"      		: "in-group-66",
	"File Sharing"	     		: "in-group-10",
	"Messaging and Chat" 		: "in-group-51",
	"CD and DVD Tools"  		: "in-group-56",
	"Drivers"		   	: "in-group-16",
	"Networking and Admin"		: "in-group-14",
	"Developer Tools"		: "in-group-8",
	"VPNs/Privacy"			: "in-group-17",
	"Browsers and Plugins"		: "in-group-5",
	"Anti-Malware"			: "in-group-2",
	"Office and News"		: "in-group-53",
	"Compression and Backup"	: "in-group-13",
	"Desktop"			: "in-group-65",
	"Audio and Video"		: "in-group-63",
	"File Transfer"			: "in-group-58",
	"Firewalls and Security"	: "in-group-59",
	"Freeware"			: "in-license-28",
	"Commercial Trial"		: "in-license-42",
	"Shareware"			: "in-license-29",
	"Open Source"			: "in-license-32",
	"Commercial Demo"		: "in-license-64",
	"Commercial Purchase"		: "in-license-61",
	"Non-Commercial Freeware"	:"in-license-44",
	"missing"			: "in-license-28",
	""				: "in-license-28",

}

user_name = """ Your user name for Wordpress """
login_pass = """ Your Passward for Wordpress Login """

browser = webdriver.Chrome()
browser.get('https://-----------.com/wp-admin/')
browser.maximize_window()

log_ = browser.find_element_by_id("user_login")
log_.send_keys(user_name)

pass_ = browser.find_element_by_id("user_pass")
pass_.send_keys(login_pass)

pass_.send_keys(Keys.ENTER)

soft = browser.find_element_by_link_text("Softwares")
soft.click()

itr = 0
error_values = dict()
for name in details.keys():
	title = details[name][0]
	category = details[name][1]
	version = details[name][2]
	auth_name = details[name][3]
	devlp_link = details[name][4]
	liscence = details[name][5]

	print(title," ",cat_data[category]," ", version, " ",auth_name," ", devlp_link, " ", liscence)
	print("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =")
	try:
		add_soft = browser.find_element_by_link_text("Add software")
		add_soft.send_keys(Keys.CONTROL , Keys.RETURN)
		browser.switch_to_window(browser.window_handles[-1])

		title_val = browser.find_element_by_id("title")
		title_val.send_keys(title)

		catgry = browser.find_element_by_id(cat_data[category])
		catgry.click()

		system_val = browser.find_element_by_id("in-system-19")
		system_val.click()

		compatabilities_val = browser.find_element_by_id("in-compatibility-40")
		compatabilities_val.click()

		liscence_val = browser.find_element_by_id(cat_data[liscence])
		liscence_val.click()

		lang_val = browser.find_element_by_id("in-language-41")
		lang_val.click()

		url = browser.find_element_by_id("filebear_size")
		url.send_keys(" ")
		url.send_keys(Keys.RETURN)

		version_val = browser.find_element_by_id("filebear_version")
		version_val.send_keys(version)

		developer = browser.find_element_by_id("filebear_developer")
		developer.send_keys(auth_name)

		url = browser.find_element_by_id("filebear_developer-url")
		url.send_keys(devlp_link)

		upload_date = browser.find_element_by_id("filebear_update-date")
		upload_date.send_keys("03-07-2019")
		url.send_keys(Keys.RETURN)
		sleep(4)
		browser.close()

		browser.switch_to_window(browser.window_handles[0])
	except:
		error_values[name] = details[name]

	itr +=1
	print(itr, " / ", len(details), " has been completed!!!")
	# if  itr == 5:
	# 	break
error_values[name] = ["This", "is", "last", "entry", "of", "error","file", "Enjoy!!!!"]

with open("erros.json" , "w") as file:
	json.dump(error_values, file, indent = 3)