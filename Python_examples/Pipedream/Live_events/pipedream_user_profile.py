import requests

headers = {
    'Authorization': 'Bearer <TODO: API_Key>',
}

response = requests.get('https://api.pipedream.com/v1/users/me', headers=headers)


print(response.text)
