# SimpleNNetwork
A Small Neural Network with a backpropagation learning algorithm,
created as a college assignment.

Made in C.

this neural network has 3 layers of 25 neurons each and 10 outputs.
It's designed to recognize "pictures" of numbers 0-9 given as a array of 25 bits each meaning a black or white pixel.
example: 
picture of a 0:

{0,1,1,1,0,

 0,1,0,1,0,
 
 0,1,0,1,0,
 
 0,1,0,1,0,
 
 0,1,1,1,0}

the outputs numbered 0-9 are the networks confidence that it is seeing a given number.

The project does not include a sufficently large set of training data to accomplish this goal, only to confirm that the algorithm is working.
