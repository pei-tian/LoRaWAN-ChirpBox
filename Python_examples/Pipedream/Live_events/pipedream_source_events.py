# TODO:
# 1. input api key (remove "<" and ">")
# 2. change "dc_OLuJQdB" to your source id

import requests

headers = {
    'Authorization': 'Bearer <api key>',
}

params = (
    ('expand', 'event'),
)

response = requests.get('https://api.pipedream.com/v1/sources/dc_OLuJQdB/event_summaries', headers=headers, params=params)

print(response.text)
