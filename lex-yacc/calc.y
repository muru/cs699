%token NUMBER
%right '+'
expr : expr '+' expr
	 | NUMBER 
	 ;

