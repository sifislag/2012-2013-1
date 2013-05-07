Συνοπτική περιγραφή των αρχείων:
	main.c: περιέχει την κύρια συνάρτηση καθώς και συναρτήσεις που φτιάχνουν καινούριο τρισδιάστατο πίνακα για το παιχνίδι ή προσθέτουν άλλη μια διάσταση σε αυτών στο τέλος κάθε γύρου
 
	functions1.c:περιέχει την συνάρτηση που δέχεται τις εντολές του χρήστη και τις εκτελεί , καθώς και την συνάρτηση που κάνει την "ανταλαγή"

	functions2.c:περιέχει συναρτήσεις που ελέγχουν αν η κατάσταση του παιχνιδιού είναι νικητήρια 

	functions3.c:περιέχει συναρτήσεις για την εκτύπωση της κατάστασης του παιχνιδιού 

	functions4.c:περιέχει την βάση της τεχνητής νοημοσύνης (αλγόριθμος minimax με "κλάδεμα" αλφαβητά) καθώς και δύο βοήθητικές προς την συνάρτηση αξιολόγησης κατάστασης συνάρτησης 

	functions5.c:περιέχει την συνάρτηση που αξιολογεί την κατάσταση του παιχνιδιού.

Για την κατασκευή εκτελέσιμου υπάρχει Makefile.


Brief description of files:
main.c: contains the main function and functions that build a new three-dimensional matrix for the game or add another dimension(round) to them at the end of each round
 
functions1.c: contains the function that receives and executes the user's commands, and the function that implements the "swap"

functions2.c: contains functions that check whether the state of the game is a winning one.

functions3.c: contains functions for printing the status of the game.

functions4.c: contains the base of the AI(minimax with alpha-beta prunning) and two functions that assist the evaluation function state parameter

functions5.c: contains the function that evaluates the state of the game(used by minimax).

Makefile included,although it isn't how it should be(found that out later).
