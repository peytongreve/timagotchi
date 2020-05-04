
import requests
import string

def request_handler(request):
	# making sure we have a valid request and username
	if request['method'] != 'POST':
		return "Not a post request"
	try:
		request['form']['username']
	except ValueError:
		return "Must enter a username"

	username = request['form']['username']
	if len(username) > 10:
		return "Username must be less than 10 characters"

	