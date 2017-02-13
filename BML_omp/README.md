# project
# (tutti i primi # sono per commentare, si arrabbia se non li metto)

# COME USARE GITHUB

# PRIMA DI TUTTO

# crea un account github (account, password, mail etc)
#poi scrivimi che ti aggiungo come collaboratore

#COME CREARE IL REPOSITORY

# avrai un repository "locale" e il repository offline

# fai una cartella chiamata project vuota, e da terminale con "cd" vai nella sua directory
#scrivi da terminale 
#   git init
#   git remote add origin https://github.com/sbrambilla777/a.git
#   git remote clone origin https://github.com/sbrambilla777/a.git

# COME GESTIRE IL REPOSITORY
# se devi scaricare il repository:
#   git pull -q origin master

# se devi aggiungere roba sul repository online:
#   git push -u origin master

# COMANDI UTILI DI GIT

# git add nomefile.cpp    (oppure git add .     per aggiungere tutta la directory) --> aggiunge nomefile.cpp al repository

# git commit nomefile.cpp  (oppure git commit . per fare il commit di tutta la director) 
# -->  commit è l'unico comando davvero importante: è quello che "salva" tutti i cambiamenti fatti finora, in nomefile.cpp o nella
# directory; ti chiedera di mettere un titolo al commit (tipo "CAMBIATO SPARSE_VECT", "AGGIUNTO FUNZIONE XXX")
# PRIMA di ogni push devi fare almeno un commit (altrimenti non fa nulla)
#in github, affianco a ogni file dovresti vedere il nome dell'ultimo commit, e con il pulsante history tutte le modifiche del 
#file tra i vari commit caricati


# git status      --> ti dice informazioni sul repository (se c'è qualcosa di modificato, qualcosa che non è dentro il repository etc)
# git log          --> ti fa vedere l'elenco dei commit che hai fatto finora
# git diff        --> ti fa vedere tutte le modifiche apportate ai file su cui non hai ancora fatto commit

