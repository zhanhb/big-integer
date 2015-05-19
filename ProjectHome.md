<ol><li>How to declare a big integer?</li>
<pre><code><br>
BigInteger x = -2147483647 - 1;<br>
BigInteger y('a'); //97<br>
BigInteger z("1234567890123"); // a huge number 1234567890123</code></pre>
<li>Simple supported operations</li>
<pre><code><br>
BigInteger a = 3, b = 4;<br>
a + b;<br>
a - b;<br>
6 + -a * +b;<br>
a % b;<br>
++a;<br>
b--;<br>
a = "+125";<br>
b = "-00002578"; //leading zeros will be ignored.<br>
a *= 100;<br>
if (a &lt; 0 || 100 == a) {<br>
a = 0;<br>
} else if (a &gt;= b) {<br>
a -= b;<br>
} else if (!b) {<br>
a += 10;<br>
} else {<br>
b = -a;<br>
}</code></pre>
<p>The values during the calculations are all BigInteger.</p>
<p><b>Notice:</b> We can not use floating number to calculate, for I don't know how to show the result, double or BigInteger? So, it's baned.</p>
<li>Input and output.</li>
<pre><code><br>
BigInteger x;<br>
char ch;<br>
cin &gt;&gt; x &gt;&gt; ch;                 // Input "123abc",<br>
cout &lt;&lt; x &lt;&lt; ' ' &lt;&lt; ch &lt;&lt; endl; // you will get "123 a".</code></pre>
<p></p>
<p>We can use BigInteger like primitive types and inner types.</p>
<p>If you are familiar with iomanip, you can use like this.</p>
<pre><code><br>
cin &gt;&gt; oct &gt;&gt; x;<br>
cout &lt;&lt; hex &lt;&lt; uppercase &lt;&lt; setw(20) &lt;&lt; setfill('.') &lt;&lt; x &lt;&lt; endl;</code></pre>
<li>Advanced supports</li>
<ol><li>Hexadecimal conversion<br>
<pre><code>BigInteger x("7fAb", 16); // 0x7fab
cout &lt;&lt; x &lt;&lt; ' ' &lt;&lt; 0x7fab &lt;&lt; endl; // 32683 32683
cout &lt;&lt; x.toString(7) &lt;&lt; endl; // 164200 (in 7 base).
</code></pre>
</li><li>bitwise operations<br>
<pre><code>int i = 50, j = -72;
BigInteger x = i, y = j;
cout &lt;&lt; (x &amp; y) &lt;&lt; ' ' &lt;&lt; (i &amp; j) &lt;&lt; endl;
cout &lt;&lt; (x ^ y) &lt;&lt; ' ' &lt;&lt; (i ^ j) &lt;&lt; endl;
cout &lt;&lt; (x | y) &lt;&lt; ' ' &lt;&lt; (i | j) &lt;&lt; endl;
</code></pre>
<li>These are what I want to implement but I can't, maybe I will finish it one day.</li>
<p><strong>The following source code will always cause an compile error.</strong></p>
</li><li>BigInteger can't cast to a boolean value<br>
<pre><code>BigInteger x;
if(x); //  BigInteger can't cast to a boolean value
</code></pre>
</li><li>BigInteger can't cast to a boolean value<br>
<pre><code>int a, b;
BigInteger x;
if(a == b || x); // same reason
</code></pre>
</li><li>Can't cast a big integer to a floating number<br>
<pre><code>BigInteger x;
double tmp = (double) x; // can't cast to a floating number
</code></pre>
You can use the following methods instead.<br>
<pre><code>BigInteger x = "123";
double tmp = x.doubleValue();
if(!!x) {
    // do something.
}
int intV = x.intValue();
</code></pre>
<li>New add functions.</li>
</li><li>intValue<br>
</li><li>doubleValue<br>
</li><li>gcd<br>
</li><li>abs<br>
</li><li>pow<br>
</li><li>swap<br>
<pre><code>BigInteger a, b, x;
x = a.gcd(b);
a.swap(b);
double d = a.doubleValue();
int ii = b.intValue();
x = x.pow(20);
b = a.abs();
</code></pre>
<li>These are what I wanna add in the coming version.</li>
</li><li>and not<br>
</li><li>is probable prime<br>
</li><li>mod inverse<br>
</li><li>mod pow<br>
</ol>
<pre><code>#include "BigInteger.h"
#include &lt;iostream&gt;
using namespace std;

int main() {
    for (BigInteger n; cin &gt;&gt; n;) {
        BigInteger fac = 1;
        for (BigInteger i = 1; i &lt;= n; ++i)
            fac *= i;
        cout &lt;&lt; fac &lt;&lt; endl;
    }
    return 0;
}
</code></pre>