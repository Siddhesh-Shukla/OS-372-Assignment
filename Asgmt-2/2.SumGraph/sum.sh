for i in {50..200}
do
   gcc read-new.c -lpthread -o read
    ./read $i
   gcc sum-new.c -lpthread -o sum
    ./sum $i
done

python3 plotting.py

# gcc read-new.c -lpthread -o read
# ./read
# gcc sum-new.c -lpthread -o sum
# ./sum