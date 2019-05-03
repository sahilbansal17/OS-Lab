# OS Lab Test (April 6, 2019)

- [Question Paper](https://drive.google.com/file/d/1OUfw6E0P7q3kRJoqg7FCZnJkI2WK6__u/view)

## Question 1
Develop a C program using two processes *Sender* and *Receiver*.

Sender reads two user arguments **name of input file** and **packet size**, reads the file, divides into packets, sends packets and a message code (is an integer in range of 0-3) to a message queue.

Receiver reads one user argument **output file**; receives the message and message code from the queue, and writes the packets as follows:

- code = 0, in hex (each byte written as two hex digits and blank)
- code = 1, as integer
- code = 2, as character
- code = 3, discards message

## Question 2
Consider a variant of **producer-consumer** problem, in which there is one **producer P** and two **consumers C1 and C2**.

Producer P writes into a buffer of finite size **N**.

C1 consumes one item at a time and C2 consumes two items at a time.

P waits if the buffer is full. C1 and C2 wait when the buffer is empty. At any time, only one of P, C1 and C2 can be in the critical section.

If buffer contains one item and C2 gets into the critical section, it exists the critical section but prevents C1 from entering next. In such a case, only P is allowed and when C2 gets chance, it shall delete two items and allow everyone to contest for the critical section.
