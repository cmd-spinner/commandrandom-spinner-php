import random
import string
import json
original_list = string.ascii_letters
current_list = string.ascii_letters
random_list = []
random_dict = {}
derandom_dict = {}
while(len(current_list)>0):
    random_letter = random.choice(current_list)
    current_list = current_list.replace(random_letter,'')
    random_list.append(random_letter)
i = 0

while(i<52):
    random_dict[original_list[i]] = random_list[i]
    derandom_dict[random_list[i]] = original_list[i]
    i = i +1
random_dict_file = open("random.json","w")
derandom_dict_file = open("derandom.json","w")
json.dump(random_dict,random_dict_file)
json.dump(derandom_dict,derandom_dict_file)