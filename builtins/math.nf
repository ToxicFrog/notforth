(Functions for math and bitwise operations.)

"math.nf" c/file

:nil 0 defconst

(Arithmetic operators. Non-commutative operators - / % expect the second operand
on top of the stack, so `10 2 /` => 5.)

:+ "push(pop() + pop());" c/defn
:* "push(pop() * pop());" c/defn
:- "Cell y = pop(); push(pop()-y); " c/defn
:/ "Cell y = pop(); push(pop()/y); " c/defn
:% "Cell y = pop(); push(pop()%y); " c/defn

(Bitwise and, or, xor, and not.)

:& "push(pop() & pop());" c/defn
:| "push(pop() | pop());" c/defn
:^ "push(pop() ^ pop());" c/defn
:~ "push(~pop());" c/defn

(Left and right shift.)

( x amount << -- x' )
:<< "Cell n = pop(); push(pop() << n);" c/defn

( x amount >> -- x' )
:>> "Cell n = pop(); push(pop() >> n);" c/defn

(Some functions implemented in notforth! bit-on? tests a bit, bit-on and bit-off
set or clear it. These functions will probably be heavily revised.)

( x bit bit-on? -- truefalse )
:bit-on? { >> 1 & } defn

( x bit bit-on -- x' )
:bit-on { 1 exch << | } defn

( x bit bit-off -- x' )
:bit-off { 1 exch << ~ & } defn

( x bit val bit-set -- x' )
(set the bit'th bit of x to val)
:bit-set {
  @bit-on exch
  @bit-off exch
  ifelse
} defn

0 c/file
