First, I opened the Inspect Element tool on Mozilla and checked out the Inspector and Network tabs. I noticed two .js files
of importance: md5.js(which implemented the MD5 hash) and index.js(which implemented the voting code). I hoped that I would
find an easy way to bypass the CPU-intensive task... However, it turned out that the CPU-intensive task was absolutely
necessary. The index.js file did the following:
<br><br>-It requested a "work" that was a string made from 6 hexadecimal letters from /vote.php?g=1.
<br>-It then tried to search for a string of length from 7 to 24 made from big and small alpha-numeric characters, such that when it was added at the end of the "watch__bisqwit__" string it would give a MD5 hash that started with the work(the string that was given by the server earlier).
<br>-Finally, it sent the request to register the vote at /vote.php, passing the arguments id for the id of the suggestion, h for the found string and u, where u was 1 if it was upvoted or 0 if it was downvoted.
<br><br>This whole process takes, I'd say, 5-10 mins. For 250 requests, it would surely be a long wait. The authors said, however, that the intended solution was meant to be done within minutes.
<br>Therefore, the only thing left to do was to somehow optimise this task. So I started looking for such ways.
<br>I thought that, if instead of searching a string for a hash at the time, I would instead compute for many strings the hashes until I found at least a string for each hash and then search in my database for them when the server asked for them, it would work much faster. There are only(given MD5 hash uses hexadecimal letters) 16^6 possibilities of "works" that can be given by the server. 16^6 =16 777 216, which sounds fast enough to generate and even possible to hold in RAM.
<br><br>However, there is a little problem. MD5 hash, as all kinds of hash, is susceptible to collisions, where two different strings have the exact same hash. Therefore, as my program would find more and more new hashes it hasn't yet saved in its database, it would run into more and more collisions, taking it a longer time to find a new hash. By the time my program would try to find the last, 16^6th "new" hash, it would take it 16^6-1 attempts. It would, in fact, be even worse than the most naive approach, as the second to last suffered nearly just as much and so did the third to last... and so on.
<br><br>However! All hope is not lost! If I were to stop at the hash before the last, it would be a tiny bit faster. It would, however, mean that when the server gave me a hash, there would be a tiny possibility that I wouldn't have a solution for this hash, requiring me to reset the work(by sending a random string to the server).
<br>I had, therefore, to find some sort of balance between the time spent generating the "database" and the time spent resetting the work gotten from the server.
<br>Of course, if I were to look into detail, it would get very ugly, as the time spent doing the first action was likely not
the same as the one spent doing the second. However, it's probably not worth going into detail. Generating only half of the
possible hashes leads to quite a nice result and my code sends 250 correct requests under 4 mins.
<br>Once the code stops running, 250 correct requests should have been sent and all you should do is refresh the main page with the suggestions to see the flag:
<br>X-MAS{NASA_aint_got_n0thin_on_m3}

<br>Now there were quite a few quirks I had run into as I was doing this challenge. Sometimes the server would send back an
empty response and the only way to stop it from sending back yet another empty response seemed to be to disconnect and
connect again. I wasn't able to find a way to get a gzip decrypter working properly, so I simply asked the server for text.
<br> It's also a good idea to make the thread sleep for a short while before sending a new request, so as to not overwhelm the server and to expect that sometimes the server may just be unavailable when the request is sent, so as to make code that
doesn't count when that happens(I've forgotten to some level to do that when I send the string, but running it two times
should do the trick if it misses some due to the server being unavailable). In order to optimise looking up a hash to see
if it's in the database or not, I transformed the hex into decimal and used a frequency vector named has that would work
this way: has\[hash]=1 if it's in the database, has\[hash]=0 if it isn't.
