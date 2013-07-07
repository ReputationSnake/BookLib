
#include "stdafx.h"
#include "DB.h"

namespace DBcore{

	void DB::delidx(int Bpos,string value){

		if(fieldtype[Bpos]!="key")return;

		int y=(int)atof(value.c_str());

		if(y==0)y=value.length();

		//cout<<"ɾ��key:"<<y<<endl;

		BTreeM.at(Bpos)->del(y);

		string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\"+field[Bpos]+".idx";

		ofstream* fout=new ofstream(filename.c_str(),ios_base::binary);

		BTreeM[Bpos]->dump(fout);

		try{
			fout->close();
		}
		catch(exception ex)
		{
			cout<<ex.what();
		}
		delete fout;
	}


	vector<string> DB::split(string value,char s){
		vector<string> res;
		int i=0;
		char ch;
		stringstream val;
		for(i=0;(unsigned int)i<value.length();i++)
		{
			ch=value.at(i);

			if(ch==s||i==value.length()-1)
			{
				res.push_back(val.str());
				val.str("");
				continue;
			}
			val<<ch;
		}
		return res;
	}


	void DB::createfrm(string frm_name){
		this->frmname=frm_name;

		if(this->dbname=="")
		{
			cout<<"error:��δѡ�����ݿ�"<<endl;
			return;
		}
		if(ifexfrm())
		{
			cout<<"error:���ļ��Ѿ�����"<<endl;
			return;
		}

		try{
			string dirname=this->dbroot+"\\"+this->dbname+"\\\\"+this->frmname+"\\\\";
			_mkdir(dirname.c_str());
			//д���µ��ļ�
			string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\data.dbs";

			ofstream fout(filename.c_str(),ios_base::binary);

			if(fout.fail())throw exception("create data file failed");

			fout.close();
			cout<<"success:�����ɹ�"<<endl;
		}
		catch(exception ex)
		{
			cout<<"error:����ʧ��\r\nԭ��:"<<ex.what()<<endl;
			throw exception("error error create table :");
		}

	}


	void DB::createfield(string frmname,vector<string> tb){
		this->frmname=frmname;
		if(this->dbname=="")
		{
			cout<<"error:��δѡ�����ݿ�"<<endl;
			throw exception("error no database selected");
			return;
		}
		this->field.clear();
		this->fieldtype.clear();
		if(!ifexfrm())return;
		//�����������
		if(ifexfield())return;
		try{
			string filename=this->dbroot+"\\"+this->dbname+"\\"+frmname+"\\"+"data.dbs";

			ofstream fout(filename.c_str(),ios_base::binary);

			int i=0;

			for(i=0;i<(int)tb.size();i++)
			{
				//cout<<tb[i]<<endl;

				fout.write(tb[i].c_str(),tb[i].length());

				field.push_back(tb[i].substr(0,tb[i].find('|')).c_str());

				if(i!=(int)tb.size()-1)fout.write("\x0b",1);

				fieldtype.push_back(tb[i].substr(tb[i].find('|')+1,tb[i].size()-1).c_str());

				if(fieldtype[i]=="key")
				{
					string filename=this->dbroot+"\\"+this->dbname+"\\"+frmname+"\\"+this->field[i]+".idx";

					ofstream* fout=new ofstream(filename.c_str(),ios_base::binary);

					fout->close();

					delete fout;
				}

				BTreeM.push_back(new BTree());

				BTreeM[i]->clear();
			}
			fout.write("\x0c",1);

			cout<<"success:�ֶδ����ɹ�"<<endl;

			fout.close();
		}
		catch(exception ex)
		{
			cout<<"error:�ֶδ���ʧ��\r\nԭ��:"<<ex.what()<<endl;
			throw exception("error error create fields :");
		}

	}

	vector<string>* DB::select(string frmname,string tdname,string value,int y){

		int i=0;

		if(y==-1)
		{
			if(this->dbname=="")
			{
				cout<<"error:��δѡ�����ݿ�"<<endl;
				throw exception("error no database selected");
				return NULL;
			}
			this->frmname=frmname;

			loadfield(frmname);

			//�������в�ѯλ��

			y=(int)atof(value.c_str());

		}

		int tablepos=-1;
		//�õ����Ӧtablepos
		for(i=0;i<(int)field.size();i++){
			if(field[i]==tdname)
			{
				tablepos=i;
				break;
			}
		}
		if(tablepos==-1)
		{
			cout<<"error:����ı���"<<endl;
			throw exception("error wrong table name");
			return NULL;
		}
		if(fieldtype[tablepos]=="key")
		{
			string valueLe=BTreeM[tablepos]->search(y);

			string fposs="";

			if(valueLe=="")
			{
				cout<<"warning:û���ҵ�"<<endl;
				return NULL;
			}
			//���������valueL�����������������õ���ÿ��value�Ĵ�С
			vector<int> valueL;

			stringstream valueS;

			for(int i=0;(unsigned int)i<valueLe.length();i++)
			{
				valueS<<valueLe[i];
				if(valueLe[i]=='#')
				{
					fposs=valueLe.substr(0,i);
					valueS.str("");
				}
				if(valueLe[i]=='|'||i==(int)valueLe.length()-1)
				{
					valueL.push_back((int)atof(valueS.str().c_str()));
					valueS.str("");
				}
			}
			valueLe="";
			//cout<<fposs;
			int fpos=(int)atof(fposs.c_str());
			//cout<<fpos;
			string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\data.dbs";

			ifstream fin(filename.c_str()); 
			//��λ
			fin.seekg(fpos);
			//��ʼ��ȡ
			//��ս��������
			this->result->clear();

			char ch=' ';

			stringstream s;

			int ii=0;

			while(!fin.eof()) {

				if((int)valueL.size()==0)break;

				if(valueL.at(0)==ii){
					//cout<<s.str()<<endl;
					this->result->push_back(s.str());
					s.str("");
					//vector<int>::iterator iter =   colors.begin();   
					valueL.erase(valueL.begin());
					ii=0;
					//����һ���ַ�
					fin.get();
				}
				ch = fin.get(); 
				s<<ch;
				ii++;
			}
			fin.close();
			if(this->result->at(tablepos)==value){
				cout<<"�ҵ�"<<endl;
				return this->result;
			}else
			{
				cout<<"waring:��ƥ��..Ѱ���½ڵ�"<<endl;
				return select(frmname,tdname,value,y+1);
			}
		}
		else
		{

			//cout<<fposs;
			int fpos=0;
			//cout<<fpos;
			string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\data.dbs";

			ifstream fin(filename.c_str()); 
			//��λ
			fin.seekg(fpos);
			//��ʼ��ȡ
			//��ս��������
			this->result->clear();

			char ch=' ';

			stringstream s;

			this->field.size();

			//��ʱ������
			vector<string> rs;
			rs.clear();
			int found=0;
			int i=0;
			while(!fin.eof()) {
				ch = fin.get();
				if(ch=='\x0B')
				{
					cout<<s.str()<<"--"<<value<<endl;
					rs.push_back(s.str());
					if(s.str()==value)
						//cout<<"wa"<<endl;
						if(tablepos==i&&s.str()==value)
						{
							found=1;
						}
						i++;
						s.str("");
				}
				else
				{
					if(ch=='\x0C')
					{
						//cout<<s.str()<<endl;
						rs.push_back(s.str());
						if(tablepos==i&&s.str()==value)
						{
							found=1;
						}
						i++;
						//����ɹ�//����ʱvector����result
						if(found){
							for(i=0;i<(int)rs.size();i++)
							{
								this->result->push_back(rs.at(i));
							}
						} 
						found=0;
						i=0;
						cout<<s.str()<<endl;
						s.str("");
						rs.clear();
					}
					else
					{
						s<<ch;
					}
				}
			}
			fin.close();

			return this->result;

		}
	}


	void DB::queryline(string queryline)
	{
		stringstream action;

		stringstream leftstring;

		int i=0;

		char ch='a';

		int p=0;

		for(i=0;(unsigned int)i<queryline.length();i++)
		{
			ch=queryline.at(i);
			if(ch==' ')
			{
				p=1;
			}
			if(p!=1)
			{
				action<<ch;
			}
			else
			{
				leftstring<<ch;
			}
		}
		//ѡ�����ݿ�
		if(action.str()=="use")
		{
			cout<<"ѡ�����ݿ�:"<<leftstring.str().substr(1,leftstring.str().length()-1)<<endl;

			this->dbname=leftstring.str().substr(1,leftstring.str().length()-1);

			return;
		}
		//�������ݿ���߱�
		if(action.str()=="create")
		{
			queryline=leftstring.str();
			leftstring.str("");
			action.str("");
			p=0;
			for(i=0;(unsigned int)i<queryline.length();i++)
			{
				ch=queryline.at(i);
				//������һ���հ�
				if(i==0&&ch==' ')continue;

				if(ch==' ')
				{
					p=1;
				}
				if(p!=1)
				{
					action<<ch;
				}
				else
				{
					leftstring<<ch;
				}
			}
			//cout<<action.str();
			if(action.str()=="database"){

				cout<<"�������ݿ�"<<this->dbroot+"\\"+leftstring.str().substr(1,leftstring.str().length()-1)<<endl;

				string dirname=this->dbroot+"\\"+leftstring.str().substr(1,leftstring.str().length()-1);

				_mkdir(dirname.c_str());

				return;
			}
			//������
			if(action.str()=="table"){
				queryline=leftstring.str();

				leftstring.str("");

				action.str("");

				p=0;
				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ�
					if(i==0&&ch==' ')continue;

					if(ch==' ')
					{
						p=1;
					}
					if(p!=1)
					{
						action<<ch;
					}
					else
					{
						leftstring<<ch;
					}
				}

				this->createfrm(action.str().substr(1,action.str().length()-2));

				vector<string> tb;

				queryline=leftstring.str();

				leftstring.str("");

				action.str("");
				p=0;
				int yyy=-1;
				string tdstring;
				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ׺�(��
					if(i==0&&ch==' ')continue;
					if(i==1&&ch=='(')continue;
					if(ch==','||ch==')')
					{
						if(p==0){
							leftstring<<action.str().substr(1,action.str().length()-1);
							yyy=action.str().find('`',1);
							if(yyy==-1)
							{
								cout<<"�﷨����"<<endl;
								return;
							}
							string tbname=action.str().substr(1,yyy-1);
							string tbtype=action.str().substr(yyy+2,action.str().length()-1);
							tdstring=tbname+"|"+tbtype;
							p=1;
						}
						else
						{
							leftstring<<action.str().substr(2,action.str().length()-1);
							yyy=leftstring.str().find('`',1);
							if(yyy==-1)
							{
								cout<<"�﷨����"<<endl;
								return;
							}
							string tbname=leftstring.str().substr(0,yyy);
							string tbtype=leftstring.str().substr(yyy+2,leftstring.str().length()-1);
							tdstring=tbname+"|"+tbtype;
						}
						//string asd=leftstring.str();
						tb.push_back(tdstring);

						action.str("");

						leftstring.str("");
					}
					action<<ch;
				}
				this->createfield(this->frmname,tb);
				//cout<<leftstring.str()<<endl;

				return;
			}
			cout<<"�﷨����"<<endl;
			return;
		}
		//�������ݿ�
		if(action.str()=="insert")
		{
			queryline=leftstring.str();

			leftstring.str("");

			action.str("");

			p=0;
			for(i=0;(unsigned int)i<queryline.length();i++)
			{
				ch=queryline.at(i);
				//������һ���հ�
				if(i==0&&ch==' ')continue;

				if(ch==' ')
				{
					p=1;
				}
				if(p!=1)
				{
					action<<ch;
				}
				else
				{
					leftstring<<ch;
				}
			}
			if(action.str()=="into"){
				queryline=leftstring.str();

				leftstring.str("");

				action.str("");

				p=0;
				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ�
					if(i==0&&ch==' ')continue;

					if(ch==' ')
					{
						p=1;
					}
					if(p!=1)
					{
						action<<ch;
					}
					else
					{
						leftstring<<ch;
					}
				}
				this->frmname=action.str().substr(1,action.str().length()-2);

				queryline=leftstring.str();

				//cout<<queryline<<endl;

				leftstring.str("");

				action.str("");

				p=0;

				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ�
					if(i==0&&ch==' ')continue;

					if(ch==' ')
					{
						p=1;
					}
					if(p!=1)
					{
						action<<ch;
					}
					else
					{
						leftstring<<ch;
					}
				}

				vector<string> tb;

				queryline=leftstring.str();

				if(action.str()!="values")
				{
					cout<<"�﷨����"<<endl;
					return;
				}

				leftstring.str("");

				action.str("");

				p=0;

				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ׺�(��

					if(i==0&&ch==' ')continue;

					if(i==1&&ch=='(')continue;

					if(ch==','||ch==')')
					{
						if(p==0){
							leftstring<<action.str().substr(1,action.str().length()-2);
							p=1;
						}
						else
						{
							leftstring<<action.str().substr(2,action.str().length()-3);
						}
						tb.push_back(leftstring.str());
						action.str("");
						leftstring.str("");
					}
					action<<ch;
				}
				this->insert(this->frmname,tb);
				//cout<<leftstring.str()<<endl;

				return;
			}

			cout<<"�﷨����"<<endl;
			throw exception("error spell");
			return;
		}
		//���ݿ��ѯ���..
		if(action.str()=="select")
		{
			queryline=leftstring.str();

			leftstring.str("");

			action.str("");

			p=0;
			for(i=0;(unsigned int)i<queryline.length();i++)
			{
				ch=queryline.at(i);
				//������һ���հ�
				if(i==0&&ch==' ')continue;

				if(ch==' ')
				{
					p=1;
				}
				if(p!=1)
				{
					action<<ch;
				}
				else
				{
					leftstring<<ch;
				}
			}
			string a=action.str();

			if(action.str()=="from"){

				queryline=leftstring.str();

				leftstring.str("");

				action.str("");

				p=0;

				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ�
					if(i==0&&ch==' ')continue;

					if(ch==' ')
					{
						p=1;
					}
					if(p!=1)
					{
						action<<ch;
					}
					else
					{
						leftstring<<ch;
					}
				}
				//����
				string frmname=action.str();
				//
				queryline=leftstring.str();
				leftstring.str("");
				action.str("");
				p=0;
				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ�
					if(i==0&&ch==' ')continue;

					if(ch==' ')
					{
						p=1;
					}
					if(p!=1)
					{
						action<<ch;
					}
					else
					{
						leftstring<<ch;
					}
				}
				//cout<<leftstring.str()<<endl;

				int eq=leftstring.str().find("=");

				int bg=leftstring.str().find(">");

				int ls=leftstring.str().find("<");

				vector<string> rs;

				//if(eq!=-1&&bg!=-1&&bg<eq){
				//	
				//}
				rs.clear();
				//����еȺ�
				if(eq!=-1)
				{
					vector<string> a=split(leftstring.str().substr(1,leftstring.str().length()),'=');

					if(ls!=-1||bg!=-1)
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-3);
					}
					else
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-2);
					}
					a.at(1)=a.at(1).substr(1,a.at(1).length()-1);
					this->select(frmname.substr(1,frmname.length()-2),a.at(0),a.at(1));
					//��ʱvector
					//��result����rs�����ʱvector
					for(i=0;i<(int)this->result->size();i++)
					{
						rs.push_back(result->at(i));
					}

				}

				if(bg!=-1)
				{
					vector<string> a;
					if(eq!=-1)
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'=');
					}
					else
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'>');
					}

					if(ls!=-1||(bg!=-1&&eq!=-1))
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-3);
					}
					else
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-2);
					}
					a.at(1)=a.at(1).substr(1,a.at(1).length()-1);

					this->selectgt(frmname.substr(1,frmname.length()-2),a.at(0),a.at(1));
					//�������=��..������ʱrs��result
					for(i=0;i<(int)rs.size();i++)
					{
						this->result->push_back(rs.at(i));
					}

					return;
				}

				if(ls!=-1)
				{
					vector<string> a;

					if(eq!=-1)
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'=');
					}
					else
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'<');
					}

					if((ls!=-1&&eq!=-1)||bg!=-1)
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-3);
					}
					else
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-2);
					}
					a.at(1)=a.at(1).substr(1,a.at(1).length()-1);

					this->selectls(frmname.substr(1,frmname.length()-2),a.at(0),a.at(1));
					//�������=��..������ʱrs��result
					for(i=0;i<(int)rs.size();i++)
					{
						this->result->push_back(rs.at(i));
					}

					return;
				}
				return;

			}

			cout<<"�﷨����"<<endl;
			throw exception("error spell");
			return;
		}
		//ɾ�����ݿ����ֶ�
		if(action.str()=="delete")
		{
			queryline=leftstring.str();
			leftstring.str("");
			action.str("");
			p=0;
			for(i=0;(unsigned int)i<queryline.length();i++)
			{
				ch=queryline.at(i);
				//������һ���հ�
				if(i==0&&ch==' ')continue;

				if(ch==' ')
				{
					p=1;
				}
				if(p!=1)
				{
					action<<ch;
				}
				else
				{
					leftstring<<ch;
				}
			}
			string a=action.str();

			if(action.str()=="from"){

				queryline=leftstring.str();

				leftstring.str("");

				action.str("");

				p=0;

				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ�
					if(i==0&&ch==' ')continue;

					if(ch==' ')
					{
						p=1;
					}
					if(p!=1)
					{
						action<<ch;
					}
					else
					{
						leftstring<<ch;
					}
				}
				//����
				string frmname=action.str();
				//
				queryline=leftstring.str();
				leftstring.str("");
				action.str("");
				p=0;
				for(i=0;(unsigned int)i<queryline.length();i++)
				{
					ch=queryline.at(i);
					//������һ���հ�
					if(i==0&&ch==' ')continue;

					if(ch==' ')
					{
						p=1;
					}
					if(p!=1)
					{
						action<<ch;
					}
					else
					{
						leftstring<<ch;
					}
				}

				//cout<<leftstring.str()<<endl;

				int eq=leftstring.str().find("=");

				int bg=leftstring.str().find(">");

				int ls=leftstring.str().find("<");

				vector<string> rs;

				//if(eq!=-1&&bg!=-1&&bg<eq){
				//	
				//}

				//����еȺ�
				if(eq!=-1)
				{
					vector<string> a=split(leftstring.str().substr(1,leftstring.str().length()),'=');

					if(ls!=-1||bg!=-1)
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-3);
					}
					else
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-2);
					}
					a.at(1)=a.at(1).substr(1,a.at(1).length()-1);

					this->remove(frmname.substr(1,frmname.length()-2),a.at(0),a.at(1));

					//��ʱvector

				}

				if(bg!=-1)
				{
					vector<string> a;
					if(eq!=-1)
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'=');
					}
					else
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'>');
					}

					if((ls!=-1&&eq!=-1)||bg!=-1)
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-3);
					}
					else
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-2);
					}
					a.at(1)=a.at(1).substr(1,a.at(1).length()-1);

					this->removegt(frmname.substr(1,frmname.length()-2),a.at(0),a.at(1));
					//�������=��..������ʱrs��result
					for(i=0;i<(int)rs.size();i++)
					{
						this->result->push_back(rs.at(i));
					}
					return;
				}

				if(ls!=-1)
				{

					vector<string> a;
					if(eq!=-1)
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'=');
					}
					else
					{
						a=split(leftstring.str().substr(1,leftstring.str().length()),'<');
					}

					if((ls!=-1&&eq!=-1)||bg!=-1)
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-3);
					}
					else
					{
						a.at(0)=a.at(0).substr(1,a.at(0).length()-2);
					}

					a.at(1)=a.at(1).substr(1,a.at(1).length()-1);

					this->removels(frmname.substr(1,frmname.length()-2),a.at(0),a.at(1));
					//�������=��..������ʱrs��result
					for(i=0;i<(int)rs.size();i++)
					{
						this->result->push_back(rs.at(i));
					}

					return;
				}

				return;

			}
			cout<<"�﷨����"<<endl;
			return;
		}
		//ɾ�����ݿ����ֶ�
		if(action.str()=="update")
		{
			queryline=leftstring.str();
			leftstring.str("");
			action.str("");
			p=0;
			for(i=0;(unsigned int)i<queryline.length();i++)
			{
				ch=queryline.at(i);
				//������һ���հ�
				if(i==0&&ch==' ')continue;

				if(ch==' ')
				{
					p=1;
				}
				if(p!=1)
				{
					action<<ch;
				}
				else
				{
					leftstring<<ch;
				}
			}
			string a=action.str();

			string frmname=action.str().substr(1,action.str().length()-2);
			//
			queryline=leftstring.str();
			leftstring.str("");
			action.str("");
			p=0;
			for(i=0;(unsigned int)i<queryline.length();i++)
			{
				ch=queryline.at(i);
				//������һ���հ�
				if(i==0&&ch==' ')continue;

				if(ch==' ')
				{
					p=1;
				}
				if(p!=1)
				{
					action<<ch;
				}
				else
				{
					leftstring<<ch;
				}

				cout<<"�﷨����"<<endl;

				return;
			}

			cout<<"�﷨����"<<endl;
			throw exception("error spell");

		}

	}

	DB::DB(string dbroot)
	{
		this->dbroot=dbroot;

		result=new vector<string>();
	}

	void DB::insert( string frmname,vector<string> row )
	{
		if(this->dbname=="")
		{
			cout<<"error:��δѡ�����ݿ�"<<endl;
			throw new exception("error no database selected");
			return;
		}
		this->frmname=frmname;

		loadfield(frmname);

		if(!ifexfrm())return;

		if(!ifexfield())return;

		if(row.size()!=this->field.size()){

			cout<<"���������"<<endl;
			return;
		}
		int y=(unsigned int)atof(row[0].c_str());

		int i=0;

		int tablepos=-1;

		for(i=0;i<(int)fieldtype.size();i++)
		{
			if(fieldtype[i]=="key")
			{
				tablepos=i;
				break;
			}
		}
		if(tablepos==-1){
			cout<<"����������"<<endl;
			throw exception("error no index file existed");
			return;
		}
		//cout<<BTreeM[0]->search(y)<<endl;
		if(BTreeM[tablepos]->search(y)!="\0"){
			cout<<"�Ѿ�����..���ʧ��"<<endl;
			throw exception("key already existed");
			return;
		}

		string filename=this->dbroot+"\\"+this->dbname+"\\"+frmname+"\\"+"data.dbs";
		//�õ��ļ���С
		ifstream f1(filename.c_str());  

		f1.seekg(0,ios::end);  

		int filesize=f1.tellg();

		f1.close();

		ofstream fout(filename.c_str(),ios_base::app);

		cout<<"����:";

		stringstream s;
		//��������
		stringstream columidx;
		for(i=0;i<(int)row.size();i++)
		{
			columidx<<row[i].length();
			if(i!=(int)row.size()-1)columidx<<'|';
		}
		//cout<<columidx.str();
		for(i=0;i<(int)row.size();i++)
		{

			//��������
			int index=(int)atof(row[i].c_str());

			if(index==0)index=row[i].length();

			s<<filesize;

			s<<'#'<<columidx.str();

			if(fieldtype[i]=="key")
			{
				while(BTreeM[i]->insert(s.str(),index)==NULL)
				{
					index++;
					//cout<<"warning:�������Ѿ�����,����1..�µ�������"<<index<<endl;
				}
			}
			s.str("");
			//��������
			if(fieldtype[i]=="key")
			{
				string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\"+this->field[i]+".idx";

				ofstream* idxout=new ofstream(filename.c_str(),ios_base::binary);

				BTreeM[i]->dump(idxout);

				idxout->close();
			}

			//cout<<row[i];
			//ע��string��ḽ��һ��\0
			fout.write(row[i].c_str(),row[i].length());

			if(i!=(int)row.size()-1)fout.write("\v",1);

		}
		cout<<"����ɹ�"<<endl;

		fout.write("\f",1);

		fout.close();
	}

	vector<string>* DB::remove( string frmname,string tdname,string value,int y/*=-1*/ )
	{
		if(this->dbname=="")
		{
			cout<<"error:��δѡ�����ݿ�"<<endl;
			throw exception("error no database selected");
			return NULL;
		}
		if(y==-1)
		{

			/*if(select(frmname,tdname,value)==NULL){
			cout<<"error:û�д˽ڵ�,,ɾ��ʧ��";
			return NULL;
			}*/
			this->frmname=frmname;

			loadfield(frmname);

			y=(int)atof(value.c_str());

			if(y==0)y=value.length();
		}
		int i=0;
		int tablepos=-1;
		//�õ����Ӧtablepos
		for(i=0;i<(int)field.size();i++){
			if(field[i]==tdname)
			{
				tablepos=i;
				break;
			}
		}
		if(tablepos==-1)
		{
			cout<<"error:����ı���"<<endl;
			return NULL;
		}
		if(fieldtype[tablepos]=="key"){
			//�������в�ѯλ��
			string valueLe=BTreeM[tablepos]->search(y);
			string fposs="";
			if(valueLe=="")
			{
				cout<<"error:û���ҵ�..�޷�ɾ��"<<endl;
				return NULL;
			}
			//���intΪ����Ҫɾ���ĳ���
			int vL=0;
			//ÿ���ؼ��ֳ���
			vector<int> valueL;
			stringstream valueS;

			for(int i=0;(unsigned int)i<valueLe.length();i++)
			{
				valueS<<valueLe[i];
				if(valueLe[i]=='#')
				{
					fposs=valueLe.substr(0,i);
					valueS.str("");
				}
				if(valueLe[i]=='|'||i==(int)valueLe.length()-1)
				{
					valueL.push_back((int)atof(valueS.str().c_str()));
					vL=vL+(int)atof(valueS.str().c_str());
					valueS.str("");
				}
			}
			vL=vL+(int)field.size()-1;
			int fpos=(int)atof(fposs.c_str());
			//cout<<fposs.c_str();
			//cout<<fpos;
			//���������ؼ���
			string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\data.dbs";
			ifstream fin(filename.c_str()); 
			//��λ
			fin.seekg(fpos);
			//��ʼ��ȡ
			//��ս��������

			this->result->clear();

			char ch=' ';
			stringstream s;
			int ii=0;
			while(!fin.eof()) {
				if((int)valueL.size()==0)break;
				if(valueL.at(0)==ii){
					//cout<<s.str()<<endl;
					this->result->push_back(s.str());
					s.str("");

					//vector<int>::iterator iter =   colors.begin();   
					valueL.erase(valueL.begin());

					ii=0;
					//����һ���ַ�
					fin.get();
				}
				ch = fin.get(); 
				s<<ch;

				ii++;
			}
			fin.close();
			//ɾ���ļ�
			filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\data.dbs";
			fstream fst(filename.c_str());

			//��Ѱ�ҵ�һ���ڵ�..�����ͬ��ɾ��.��ͬ��˵��ԭ�ڵ��Ѿ���ɾ���������ڵ�ռ��..Ѱ����һ��key

			if(value!=this->result->at(tablepos))
			{
				return remove(frmname,tdname,value,y+1);
			}
			//�ض�λ
			fst.seekg(fpos);

			//cout<<fpos<<"--"<<vL<<endl;

			ii=0;

			while(ii<vL) {
				fst.put('\xEE');
				ii++;
			}
			cout<<"key:"<<this->result->at(tablepos)<<"ɾ���ɹ�"<<endl;

			fst.close();

			//ii=0;
			//��������ɾ��
			//for(ii=0;(unsigned int)ii<this->result->size();ii++)
			{
				//cout<<;
				this->delidx(tablepos,this->result->at(0));
			}
			return NULL;
		}
		else
		{

			//cout<<fposs;
			int fpos=0;
			//cout<<fpos;
			string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\data.dbs";

			fstream fin(filename.c_str()); 
			//��λ
			fin.seekg(fpos);
			//��ʼ��ȡ
			//��ս��������
			this->result->clear();

			char ch=' ';

			stringstream s;

			this->field.size();
			//ǰ��λ��
			int prelenth=0;
			//�г���
			int rowlen=0;

			int found=0;

			int i=0,j=0;

			while(!fin.eof()) {
				ch = fin.get();
				rowlen++;
				if(ch=='\x0B')
				{
					cout<<s.str()<<"--"<<value<<endl;

					if(tablepos==i&&s.str()==value)
					{
						found=1;
					}
					i++;
					s.str("");
				}
				else
				{
					if(ch=='\x0C')
					{
						cout<<s.str()<<endl;
						if(tablepos==i&&s.str()==value)
						{
							found=1;
						}
						i++;
						//����ɹ�ɾ��
						if(found){
							cout<<"֮ǰ"<<prelenth<<endl;
							fin.seekg(prelenth);
							for(j=0;j<rowlen-1;j++)
							{
								fin.write("#",1);
							}
						} 
						prelenth+=rowlen;
						fin.seekg(prelenth);
						cout<<rowlen<<endl;
						found=0;
						i=0;
						rowlen=0;
						s.str("");
					}
					else
					{
						s<<ch;
					}
				}
			}
			fin.close();

			return this->result;
		}
	}

	bool DB::ifexfield()
	{
		if((int)field.size()!=0)
		{
			return true;
		}
		cout<<"error:���ֶ�����"<<endl;
		//throw exception("error no fields");
		return false;
	}

	bool DB::loadfield( string frmname )
	{
		this->field.clear();
		this->fieldtype.clear();
		this->BTreeM.clear();
		string filename=this->dbroot+"\\"+this->dbname+"\\"+frmname+"\\data.dbs";

		ifstream fin(filename.c_str(),ios_base::binary);

		if(fin.fail())
		{
			fin.close();
			cout<<"error:�ļ���ʧ��"<<endl;
			throw exception("error files");
			//throw exception("�ֶ���������");
			//����Ѿ�����Ĵ���ı�
			this->field.clear();
			this->fieldtype.clear();
			return false;
		}
		char ch;

		stringstream s;

		string value="";

		int i=0;
		while(!fin.eof()) {
			ch = fin.get(); 
			s<<ch;
			if (ch == '\x0b')
			{	
				value=s.str().substr(0,s.str().length()-1);
				//cout<<"tips:�����ֶ�:"<<value<<endl;
				this->field.push_back(value.substr(0,value.find('|')));
				this->fieldtype.push_back(value.substr(value.find('|')+1,value.length()));
				//��ȡ�ֶε�����
				loadindex(value.substr(0,value.find('|')),i);
				s.str("");
				i++;
			}
			if (ch == '\x0c')
			{
				//cout<<s.str()<<endl;
				//���s,,�����浽value
				value=s.str().substr(0,s.str().length()-1);

				//cout<<"tips:�����ֶ�:"<<value<<endl;
				this->field.push_back(value.substr(0,value.find('|')));
				this->fieldtype.push_back(value.substr(value.find('|')+1,value.length()));
				//��ȡ�ֶε�����
				loadindex(value.substr(0,value.find('|')),i);

				s.str("");

				i++;

				return true;
			}
		}
		this->field.clear();
		this->fieldtype.clear();

		cout<<"error:���в������ֶ�..�����Ǵ�����ļ�"<<endl;
		throw exception("error files");
		return false;
	}

	bool DB::ifexfrm()
	{
		if(this->frmname!="")
		{
			string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\data.dbs";
			ifstream fin(filename.c_str(),ios_base::binary);
			if(fin.fail())
			{
				fin.close();
				cout<<"error:������"<<endl;
				//throw exception("error no table existed");
				//throw exception("������");
				return false;
			}
			return true;
		}
		else
		{
			cout<<"error:��δѡ���"<<endl;
			throw exception("error no table selected");
			//throw exception("��δѡ���");
			return false;
		}
	}

	void DB::loadindex( string index_filename,int i )
	{
		if(fieldtype[i]!="key")
		{
			BTreeM.push_back(new BTree());
			BTreeM[i]->clear();
			return;
		}
		if(!ifexfrm())return;
		string filename=this->dbroot+"\\"+this->dbname+"\\"+this->frmname+"\\"+index_filename+".idx";
		ifstream* fin=new ifstream(filename.c_str(),ios_base::binary);
		BTreeM.push_back(new BTree());
		BTreeM[i]->clear();
		int y=BTreeM[i]->load(fin);/*
								   cout<<BTreeM[i]->searchNode(1)<<endl;
								   cout<<BTreeM[i]->searchNode(2)<<endl;
								   cout<<BTreeM[i]->searchNode(3)<<endl;
								   cout<<BTreeM[i]->searchNode(4)<<endl;
								   cout<<BTreeM[i]->searchNode(5)<<endl;*/
		cout<<"tips:��������"<<this->field[i]<<"��"<<y<<"��"<<endl;
	}

	vector<string>* DB::selectall( string frmname,string tdname,vector<string> valueAl )
	{
		vector<string>* rs=new vector<string>();

		rs->clear();

		int i=0,j=0;

		for(i=0;i<(int)valueAl.size();i++)
		{
			select(frmname,tdname,valueAl[i]);
			//����result�Ľ���rs
			for(j=0;j<(int)this->result->size();j++)
			{
				rs->push_back(result->at(j));
			}
		}
		//��ս����
		this->result->clear();
		//����ʱ���ݴ���result
		for(i=0;i<(int)rs->size();i++)
		{
			this->result->push_back(rs->at(i));
		}

		delete rs;

		return this->result;
	}

	void DB::removeall( string frmname,string tdname,vector<string> valueAl )
	{
		int i=0,j=0;

		for(i=0;i<(int)valueAl.size();i++)
		{
			//cout<<valueAl.at(i)<<endl;
			this->remove(frmname,tdname,valueAl[i]);
		}
		//��ս����
		this->result->clear();

		return;
	}

	vector<string>* DB::selectgt( string frmname,string tdname,string minvalue,int y/*=-1*/ )
	{
		int i=0;

		if(y==-1)
		{
			if(this->dbname=="")
			{
				cout<<"error:��δѡ�����ݿ�"<<endl;
				throw exception("error no database selected");
				return NULL;
			}
			this->frmname=frmname;

			loadfield(frmname);

			//�������в�ѯλ��

			y=(int)atof(minvalue.c_str());

		}

		int tablepos=-1;
		//�õ����Ӧtablepos
		for(i=0;i<(int)field.size();i++){
			if(field[i]==tdname)
			{
				tablepos=i;
				break;
			}
		}
		if(tablepos==-1)
		{
			cout<<"error:����ı���"<<endl;

			throw exception("error table name");

			return NULL;
		}

		vector<string> valueGt=BTreeM[tablepos]->searchNode_gt(y);

		return this->selectall(frmname,tdname,valueGt);
	}

	vector<string>* DB::removegt( string frmname,string tdname,string minvalue,int y/*=-1*/ )
	{
		int i=0;

		if(y==-1)
		{
			if(this->dbname=="")
			{
				cout<<"error:��δѡ�����ݿ�"<<endl;
				throw exception("error no database selected");
				return NULL;
			}
			this->frmname=frmname;

			loadfield(frmname);

			//�������в�ѯλ��

			y=(int)atof(minvalue.c_str());
		}

		int tablepos=-1;
		//�õ����Ӧtablepos
		for(i=0;i<(int)field.size();i++){
			if(field[i]==tdname)
			{
				tablepos=i;
				break;
			}
		}
		if(tablepos==-1)
		{
			cout<<"error:����ı���"<<endl;

			return NULL;
		}

		vector<string> valueGt=BTreeM[tablepos]->searchNode_gt(y);

		this->removeall(frmname,tdname,valueGt);

		return this->result;
	}

	vector<string>* DB::removels( string frmname,string tdname,string maxvalue,int y/*=-1*/ )
	{
		int i=0;

		if(y==-1)
		{
			if(this->dbname=="")
			{
				cout<<"error:��δѡ�����ݿ�"<<endl;
				throw exception("error no database selected");
				return NULL;
			}
			this->frmname=frmname;

			loadfield(frmname);

			//�������в�ѯλ��

			y=(int)atof(maxvalue.c_str());

			if(y==0)y=maxvalue.length();
		}

		int tablepos=-1;
		//�õ����Ӧtablepos
		for(i=0;i<(int)field.size();i++){
			if(field[i]==tdname)
			{
				tablepos=i;
				break;
			}
		}
		if(tablepos==-1)
		{
			cout<<"error:����ı���"<<endl;

			return NULL;
		}

		vector<string> valueLe=BTreeM[tablepos]->searchNode_le(y);

		this->removeall(frmname,tdname,valueLe);

		return this->result;
	}

	vector<string>* DB::selectls( string frmname,string tdname,string maxvalue,int y/*=-1*/ )
	{
		int i=0;

		if(y==-1)
		{
			if(this->dbname=="")
			{
				cout<<"error:��δѡ�����ݿ�"<<endl;
				throw exception("error no database selected");
				return NULL;
			}
			this->frmname=frmname;

			loadfield(frmname);

			//�������в�ѯλ��

			y=(int)atof(maxvalue.c_str());

			if(y==0)y=maxvalue.length();
		}

		int tablepos=-1;
		//�õ����Ӧtablepos
		for(i=0;i<(int)field.size();i++){
			if(field[i]==tdname)
			{
				tablepos=i;
				break;
			}
		}
		if(tablepos==-1)
		{
			cout<<"error:����ı���"<<endl;

			return NULL;
		}

		vector<string> valueLe=BTreeM[tablepos]->searchNode_le(y);

		return this->selectall(frmname,tdname,valueLe);
	}
}