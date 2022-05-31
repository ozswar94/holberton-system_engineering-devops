#!/usr/bin/python3
""" handle reddit api """
import requests


def number_of_subscribers(subreddit):
    """ this function return the number of subcribers by subreddit """
    header = {'user-agent': 'APP-Subreddits'}
    url = 'https://www.reddit.com/r/{}/about.json'.format(subreddit)
    response = requests.get(url, headers=header)
    if response.status_code > 300:
        return None
    return response.json().get("data").get('subscribers')
