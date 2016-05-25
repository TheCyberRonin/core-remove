crmake: coreRemove.c
	gcc -o crmake coreRemove.c -I.
test:
	crmake .

