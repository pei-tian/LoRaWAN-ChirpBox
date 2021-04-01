# TODO:
# 1. input api key (remove "<" and ">")

import requests

headers = {
    'Authorization': 'Bearer <api key>',
}

response = requests.get('https://api.pipedream.com/v1/users/me', headers=headers)


print(response.text)
