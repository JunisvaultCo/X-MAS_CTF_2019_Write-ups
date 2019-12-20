First, I started by installing netcat and fiddled around for a while with the values.
Trying to think functions that would give similar answers... I failed to see such functions.
Soon, I realised that other numbers than 0 and 1 were never good guesses. And 31 was the maximum number any of the values
passed onto the functions could have been.
So I began thinking that writting a simple code that runs through all 961 values and stores whether the guess is 0 is the
whole program.
Well, that wasn't all. Once I did that, I ran into a beautiful "Great! You did it! Now what?". So, of course, I asked
myself again why the challenge is named "Function Plotter". I thought, f(x,y)=1 if the point is actually plotted, and
f(x,y)=0 if it isn't.
I outputted the matrix. I couldn't see a pattern, so I decided to make the 1s and 0s more distinct. # for 1 and _ for 0.
This revealed a QR code. Since it wasn't yet in a good enough shape, I wrote a Java program, as I wasn't accustomed enough
to graphics in C++ and I wrote the QR code into an actual image, which I simply decoded at https://zxing.org/w/decode.jspx ,
which turned out to reveal the flag:

X-MAS{Th@t's_4_w31rD_fUnCt10n!!!_8082838205}
