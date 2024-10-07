# Modes of operation

Block ciphers are able to run in different modes of operation
which allow for different scenarios; ie: bank sending TB of data, realtime messaging

Five common modes of operation
1. *ECB*    Electronic Codebook
2. *CBC*    Cipher Block Chaining
3. *CFB*    Cipher Feedback
4. *OFB*    Output Feedback
5. *CTR*    Counter

## Electronic Codebook
Basic mode of operation for block ciphers
Breaks Plaintext up into sized blocks and each
are processed seperatley by the encryption function Ek

P = [P1, P2, P3, ..., Pl]
C = [C1, C2, C3, ..., Cl]

where *l* is the number of chunks, *P* is the plaintext and *C* is th ciphertext
can be exploited if someone is able to observe communication between 2 parties and
record both cipher and corresponding plain texts in a *codebook*
this *codebook* can be used to decipher future communication, without a key *K*
repeated messages or formatting can become a vulnerbility

another issue is that if the attacker has a codebook, they may be able to 
extract portions of message and construct false message with CT in codebook

## Cipher Block Chaining

