# facebookEventGenderize
Sort list of attending people on facebook event by gender

1) Get the facebook event ID on the event URL 
2) Go on https://developers.facebook.com/ and click Graph API Explorer in 'Tools & Support'
3) Get an access token and make the request with your event ID  ex:
 1598502303705794/attending
 (For big event, +1000 attending, you can add the limit option ex: 1598502303705794/attending?limit=10000)
 4) Copy the entire list and paste in 'attending.txt'
 5) Run the program
 
 ## The program will compare each person name with 2 dictionary       ##
 ## If not found, it will make a request to an API (api.genderize.io) ##
 ## and add it to the local dictionary for the next time              ##
 ## At the end, it will let you sort unrecognized people              ##
