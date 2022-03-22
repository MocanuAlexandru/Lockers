
		Sisteme de Operare: Proiect de Laborator 
	
	
		Grupa: 235
	
		Echipa: Sotir Anca-Nicoleta, Mocanu Alexandru
	
		Cerinta: Lockers									
			
	
	I. Link-area bibliotecii:													
																
		1. Interfata - header:	my_locks.h
		
		2. Implementare:		my_locks.c
		
		3. Library Object File: my_locks.o
			gcc -o my_locks.o -c my_locks.c -fPIC
		
		4. Shared Object File:	libmy_locks.so
			gcc -shared -o libmy_locks.so my_locks.o -lm
		
		5. Pentru a putea gasi .so la link-are:
			export LD_LIBRARY_PATH=<path_to_project>/Implementari:$LD_LIBRARY_PATH
			
		6. Compilarea unui test, de exemplu test_mtx.c, din folderul Lockers/Teste:
			gcc test_mtx.c -o test_mtx -pthread -L../Implementari -lmylocks
	
		
	II. Teste:
		
		-	test_mtx.c :	Exemplu utilizare mutex
		-	test_sem.c :	Exemplu utilizare Semafor
		-	test_rw.c  :	un scriitor scrie elementele de la 1 la N intr-un vector; K cititori cauta in vector, fiecare cate un numar diferit


