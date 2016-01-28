class StmtAst
{
public:
	int key;
	virtual void print()
	{
		if(key == 0)
		{
			
		}
	}
};

class ExpAst
{
public:
	virtual void print();
};

class empty_StmtAst: public StmtAst
{
	print()
	{
		cout<<"Empty";
	}
};

class seq_StmtAst : public StmtAst
{
public:
	StmtAst s;
	print()
	{
		cout<<"["<<s.print()<<"]";
	}
};

class Ass_StmtAst : public StmtAst
{
public:
	ExpAst a, b;
	print()
	{
		cout<<
	}
};