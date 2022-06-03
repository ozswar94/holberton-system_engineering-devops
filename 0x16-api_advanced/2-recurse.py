#!/usr/bin/python3
""" handle reddit api """


import requests


def recurse(subreddit, hot_list=[], after=""):
    """ recurse querie """
    header = {'user-agent': 'APP-Subreddits'}
    payload = {'after': after}
    url = 'https://www.reddit.com/r/{}/hot.json'.format(subreddit)
    response = requests.get(url, headers=header, params=payload)
    if response.status_code > 300:
        return None
    ret = response.json().get("data").get("children")
    after = response.json().get('data').get('after')
    for i in ret:
        hot_list.append(i.get('title'))
    if after is not None:
        return recurse(subreddit, hot_list, after)
    return hot_list
