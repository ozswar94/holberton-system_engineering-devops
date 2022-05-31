#!/usr/bin/python3
""" handle reddit api """
import requests


def top_ten(subreddit):
    """this function return the top 10 in subreddit"""
    header = {'user-agent': 'APP-Subreddits'}
    payload = {'limit': 10}
    url = 'https://www.reddit.com/r/{}/hot.json'.format(subreddit)
    response = requests.get(url, headers=header, params=payload)
    if response.status_code > 300:
        return None
    ret = response.json().get("data").get("children")
    for i in range(10):
        print(ret[i].get('data').get('title'))
