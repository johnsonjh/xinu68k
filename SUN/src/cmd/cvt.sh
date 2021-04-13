: 'cvt -- convert to LSI 11 code by twiddling the mfps and mtps'
:
: 'cvt filename'
:
PATH=/bin:/usr/bin
sed "s/~ps=/ ~ps= /" < $1 |
awk ' {if ($2 == "~ps")
		printf("%s %s(r5) %s\n",$1,val,$3);
       else if ($3 == "~ps")
                printf("%s %s %s(r5)\n",$1,$2,val);
       else {
          if ($1 == "~ps=")
                val = $2;
          else if ($2 == "~ps=")
                val = $3;
          print;
       }
      }'
