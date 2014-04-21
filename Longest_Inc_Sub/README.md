<h2>Longest Incrememting Sub-Sequence Problem</h2>

<p>The problem we want to solve is given some sequence <b>S = { 6, 2, 3, 6, 5, 8 }</b> what is the longest subsequence that is increasing? So for example some incresing subsequences would be : </p>
<ul>
<li>{ 6, 9 }</li>
<li>{ 3, 6, 8 }</li>
<li><b>{ 2, 3, 5, 8 }</b></li>
</ul>

<p>Where the solution would be the longest is length 4 with the sub-sequence { 2, 3, 5, 8 }. While we can do this in a brute force way by checking all possible subsequences it would take <b>O(2^n)</b>, which is simply too inefficent. We can use dynamic programing to solve this in <b>O(n^2)</b>, a much better solution.</p>

<h4>Details</h4>
<p>To solve the problem using dynamic programing what we do is go through the list starting from the first value, then moving through and finding the longest subsequence like so:</p>

<pre> [ X | . | . | . | . ]  =>  [ X | X | . | . | . ] =>  [ X | X | X | . | . ] </pre>

We will make a new array called <b>L</b> and store the sub-problems that find the longest incresing subsequences of each value coorisponding to the origonal array <b>S</b> index. The first item is only a single value array, hence it's sequence is simply 1.

<pre>
 S = [ 6 | 2 | 3 | 6 | 5 | 8 ]

 L = [ 1 | . | . | . | . | . ]  =>  [ 1 | 1 | . | . | . | . ] =>  [ 1 | 1 | 2 | . | . | . ] => [ 1 | 1 | 2 | . | . | . ]   =>  [ 1 | 1 | 2 | 3 | . | . ]  =>  [ 1 | 1 | 2 | 3 | 3 | . ] =>  [ 1 | 1 | 2 | 3 | 3 | 4 ] 
</pre>

Now the maximum value from L is our longest subsequence. To get the sequence itself we start from the max value in L let's call it <b>i</b>, then find <b>i-1</b> , <b>i-2</b> , ... , 0 , and plug them back into our normal S matrix. Then if we want to we can reverse that array to have then in order. So from our example find were the value goes down by one, then is the next item in our sequence. [ 1 | <b>1</b> | <b>2</b> | 3 | <b>3</b> | <b>4</b> ] => 2,3,5,8.
