Bon dia,
Apunts de l'entrega. El keyboard y rellotge funcionen. El gettime també tot i que no esigui al codi usuari. El write amb trucada int funciona perfectament y amb sysenter també, l'unic
que amb sysenter al acabar d'escriure salta una excepció. Es estrany perquè despres provo a executar amb el gdb pas a pas i no em salta l'excepcio. Despres, la exepció de page fault 
funciona i paso com a parametre l'adreça que es l'eip, pero no puc usar les funcions de libc a sistema no se per que. He provat de incloure els arxius .o necessaris al Makefile 
pero em donava alguns errors. Donat que m'estic quedant sense temps ho entrego tal com esta.
Gracies
