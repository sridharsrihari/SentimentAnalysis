# SentimentAnalysis
Twitter Sentiment Analysis using CPPRest Library/

The cpp file is included in the ServerClient Directory. It uses the cpprest packages found inside the Packages folder. The way I was able
to add these to the project was using a package manager called nuget which works inside of Visual Studio. Additionally, it also required
manually adding the target libraries for it to work. This required modifying paths to the external libraries in the project properties.
Once it starts working, you would mostly want to stick to the post_sentiment_analysis method. This takes in JSON request objects, connects
to the sentiment 140 API, sends the request and receives the JSON response object. In addition to the POST method, handlers and dummy methods
have also been created for GET, PUT and delete methods. These currently work on a test vector of strings.

GET - Retrieves all values currently stored in the vector.
PUT - Adds a new string.
DELETE - Delets the requested string if present. 

In order to define a new method to handle a request, just define it and then reference it inside of the listener.support() call in main
for the corresponding request type.  
