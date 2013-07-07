//B��������cpp�ļ�
//
//by baizhongwei
//blog http://haku.webcpp.net
#include "stdafx.h"
#include "BTreeM.h"

int BTree::count(BTreeNode* p)
{
		BTreeNode* p2=p;
		//����ǰ����
		int p_i=0;
		while(p2->pre)
		{
			p_i++;
			p2=p2->pre;
		}
		//�������
		int n_i=0;
		while(p->next)
		{
			n_i++;
			p=p->next;
		}
		return p_i+n_i+1;
	}

BTree::BTree(){
		this->root=NULL;
	}

 BTree::BTree(string rootvalue,int rootkey){
		
		this->root=new BTreeNode(rootvalue,rootkey);
	
	}

BTreeNode* BTree::del(BTreeNode* p){

		if(p==NULL) return 0;

		int  up=0;
		//����Һ���Ϊ��������Ϊ��.ֱ��ɾ��
		if(p->leftchild==NULL&&p->rightchild==NULL)
		{
			int now=p->key;

			if(p->parent!=NULL)
			{
				BTreeNode* h=p->parent;

				while(h)
				{

					//���key����now
					//40       +42+
					//     41
					if(h->key>now&&h->leftchild==p)
					{
						h->leftchild=p->next;
						if(h->pre)h->pre->rightchild=p->next;
						up=1;
						break;
					}
					//�α�����
					if(h->next)
					{
						h=h->next;
					}
					else
					{
						break;
					}
				}
				if(up!=1&&h->rightchild==p)
				{
					h->rightchild=p->next;
					if(h->next)h->next->leftchild=p->next;
				}

			}
			if(p->parent==NULL)
			{
				this->root=NULL;
				if(p->pre)
				{
					this->root=p->pre;
				}
				if(p->next)
				{
					this->root=p->next;
				}
			}
			//�������ǰ��..����������.
			if(p->pre)
			{
				p->pre->next=p->next;
			}
			//������ں���..����������.
			if(p->next)
			{
				p->next->pre=p->pre;
			}
			return p;
		}
		//������Ӳ�Ϊ�����Һ���Ϊ��..����֧����..Ȼ�����֮
		if(p->leftchild!=NULL&&p->rightchild==NULL)
		{
			BTreeNode* r=p->leftchild;
			int now=p->key;
			r->parent=p->parent;
			while(r->next)
			{
				r->parent=p->parent;
				r=r->next;
			}
			r->parent=p->parent;

			if(p->parent!=NULL)
			{
				BTreeNode* h=p->parent;

				while(h)
				{

					//���key����now
					//40       +42+
					//     41
					if(h->key>now&&h->leftchild==p)
					{
						h->leftchild=p->next;
						if(h->pre)h->pre->rightchild=p->next;
						up=1;
						break;
					}
					//�α�����
					if(h->next)
					{
						h=h->next;
					}
					else
					{
						break;
					}
				}
				if(up!=1&&h->rightchild==p)
				{
					h->rightchild=p->next;
					if(h->next)h->next->leftchild=p->next;
				}

			}
			if(p->parent==NULL)
			{
				this->root=NULL;
				if(p->pre)
				{
					this->root=p->pre;
				}
				if(p->next)
				{
					this->root=p->next;
				}
			}
			//�������ǰ��..����������.
			if(p->pre)
			{
				p->pre->next=p->leftchild;
				p->pre->rightchild=p->leftchild->leftchild;
			}
			//������ں���..����������.
			if(p->next)
			{
				p->next->pre=r;
				p->next->leftchild=r->rightchild;
			}
			if(p->parent==NULL)
			{
				this->root=p->leftchild;
				splitnode(this->root,1);
			}
			return p;
		}

		//����Һ��Ӳ�Ϊ��������Ϊ��..����֧����..Ȼ�����֮
		if(p->leftchild==NULL&&p->rightchild!=NULL)
		{

			BTreeNode* r=p->rightchild;

			int now=p->key;
			//���ø��ڵ�
			r->parent=p->parent;
			while(r->next)
			{
				r->parent=p->parent;
				r=r->next;
			}
			r->parent=p->parent;

			if(p->parent!=NULL)
			{
				BTreeNode* h=p->parent;

				while(h)
				{

					//���key����now
					//40       +42+
					//     41
					if(h->key>now&&h->leftchild==p)
					{
						h->leftchild=p->next;
						if(h->pre)h->pre->rightchild=p->next;
						up=1;
						break;
					}
					//�α�����
					if(h->next)
					{
						h=h->next;
					}
					else
					{
						break;
					}
				}
				if(up!=1&&h->rightchild==p)
				{
					h->rightchild=p->next;
					if(h->next)h->next->leftchild=p->next;
				}

			}
			if(p->parent==NULL)
			{
				this->root=NULL;
			}
			//�������ǰ��..����������.
			if(p->pre)
			{
				p->pre->next=p->rightchild;
				p->pre->rightchild=p->rightchild->leftchild;
			}
			//������ں���..����������.
			if(p->next)
			{
				p->next->pre=r;
				p->next->leftchild=r->rightchild;
			}
			if(p->parent==NULL)
			{
				this->root=p->leftchild;
				splitnode(this->root,1);
			}
			if(p->parent==NULL)
			{
				this->root=p->rightchild;
				splitnode(this->root,1);
			}
			return p;
		}
		//����Һ��Ӳ�Ϊ�������Ӳ�Ϊ��..��������֧�����һ�����ĵݹ�
		if(p->leftchild!=NULL&&p->rightchild!=NULL)
		{
			BTreeNode* r=p->leftchild;

			BTreeNode* w=NULL;

			while(r->next)
			{
				r=r->next;
			}

			while(r->rightchild!=NULL)
			{
				while(r->next)
				{
					r=r->next;
				}
				if(r->rightchild!=NULL)break;

				r=r->rightchild;
			}
			w=r;
			if(r->rightchild)r=r->rightchild;
			//ת�ƽڵ�
			if(r->pre)r->pre->next=NULL;
			//����ɾ��֮
			if(w->rightchild==r){w->rightchild=NULL;}
			//���ƶ��ڵ������
			if(p->leftchild!=r)r->leftchild=p->leftchild;
			r->rightchild=p->rightchild;
			r->pre=p->pre;
			r->next=p->next;
			r->parent=p->parent;
			if(p->next)p->next->pre=r;
			if(p->pre)p->pre->next=r;
			if(p->leftchild&&p->leftchild!=r)
			{

				BTreeNode* s=p->leftchild;

				while(s)
				{
					s->parent=r;
					s=s->next;
				}
			}
			if(p->rightchild){

				BTreeNode* s=p->rightchild;

				while(s)
				{
					s->parent=r;
					s=s->next;
				}
			}
			if(p->parent==NULL||p==root){

				this->root=r;

				return p;
			}
			if(p->parent->leftchild==p)
			{
				p->parent->leftchild=r;
			}
			if(p->parent->rightchild==p)
			{
				p->parent->rightchild=r;
			}
			return p;

		}


		return NULL;
	}

bool BTree::del(int key){

	BTreeNode* p=searchNode(root,key);
	
	if(p==NULL) return 0;
	/*p->parent=new BTreeNode("321",5);
	this->root=p->parent;*/

	p=del(p);

	if(p!=NULL)delete p;

	return 1;
 }

BTreeNode* BTree::insert(string value,int key)
	{
		if(this->root==NULL)
		{
			this->root=new BTreeNode(value,key);
			return this->root;
		}
		return addnode(root,value,key);
	}


 void BTree::splitnode(BTreeNode* p,int m)
	{
		if(m==1&&count(p)>2)
		{
			//cout<<"�����Ǹ��ڵ�..����2..����ڵ�"<<endl;
			//�ҵ������м�Ľڵ�
			while(p->pre)p=p->pre;

			BTreeNode* p_mid=p->next;

			BTreeNode* p_r=p->next->next;

			p_mid->leftchild=p;
			p_mid->rightchild=p_r;
			p_mid->pre=NULL;
			p_mid->next=NULL;
			//��ڵ�����
			p->parent=p_mid;
			p->next=NULL;
			BTreeNode* vec=NULL;
			vec=p->leftchild;
			while(vec)
			{
				vec->parent=p;
				vec=vec->next;
			}
			vec=p->rightchild;
			while(vec)
			{
				vec->parent=p;
				vec=vec->next;
			}
			//�ҽڵ�����
			p_r->parent=p_mid;
			p_r->pre=NULL;
			vec=p_r->leftchild;
			while(vec)
			{
				vec->parent=p_r;
				vec=vec->next;
			}
			vec=p_r->rightchild;
			while(vec)
			{
				vec->parent=p_r;
				vec=vec->next;
			}
			//�¸��ڵ�
			this->root=p_mid;
		}
		else{
			if(m!=1&&count(p)>m){
				//cout<<m<<"���ڵ����..����ڵ�"<<endl;
				//cout<<"ȡ�м�ڵ�..λ��:"<<m/2<<endl;
				//���p
				int i=0;
				//��ڵ�
				while(p->pre)p=p->pre;
				BTreeNode* p_mid=p;
				//�м�ڵ�
				while(i++<m/2&&p_mid->next)
				{
					//cout<<i<<endl;
					p_mid=p_mid->next;
				}
				BTreeNode* p_r=p_mid->next;
				//��ڵ�����
				//���¸��ڵ�
				BTreeNode* pt=p;
				while(pt->next!=NULL&&pt!=p_mid)
				{
					pt->parent=p_mid;
					pt=pt->next;
				}
				p_mid->pre->next=NULL;
				//�ҽڵ�����
				//���¸��ڵ�
				pt=p_mid->next;
				while(pt!=NULL)
				{
					pt->parent=p_mid;
					pt=pt->next;
				}
				if(p_r!=NULL)p_r->pre=NULL;
				//�м�ڵ�����
				p_mid->leftchild=p;
				p_mid->rightchild=p_r;
				p_mid->pre=NULL;
				p_mid->next=NULL;
				//�޸��м�ڵ�ĸ��ڵ�
				BTreeNode* p_p=p_mid->parent;
				/*�������������..�����*/
				p_mid->parent=p_p->parent;
				//���ϲ�Ѱ��pӦ�ò����λ��
				int key_p=0;
				i=0;
				while(p_p)
				{
					key_p=p_p->key;
					//�����һ���ͱ�key��..��ֱ�Ӳ���
					if(i==0&&key_p>p_mid->key)
					{
						//cout<<key_p<<endl;
						p_mid->next=p_p;
						p_p->pre=p_mid;
						p_p->leftchild=p_mid->rightchild;
						if(p_p==this->root)this->root=p_mid;
						splitnode(p_p,m-1);
						break;
					}
					//������ڽڵ��key���м�ڵ�keyС
					//��������key������м�ڵ�
					if(key_p<p_mid->key)
					{
						//cout<<key_p<<endl;
						BTreeNode* p_pn=p_p->next;
						p_p->next=p_mid;
						p_mid->pre=p_p;
						p_p->rightchild=p_mid->leftchild;
						p_mid->leftchild->parent=p_p;
						if(p_pn){
							p_pn->leftchild=p_mid->rightchild;
							p_mid->next=p_pn;
							p_pn->pre=p_mid;
							p_mid->rightchild->parent=p_pn;
							p_mid=p_pn;
						}
						//cout<<m<<endl;
						splitnode(p_p,m-1);
						break;
					}
					p_p=p_p->next;
					i++;
				}
			}
		}
	}

	BTreeNode* BTree::addnode(BTreeNode* p,string value,int key,int m){
		//���p�ǿսڵ�..���ؿ�
		if(p==NULL)
		{
				return p;
		}
		int now_key=p->key;
		//������..�򷵻ؿ�
		if(now_key==key)
		{
			//throw exception("�Ѿ����ڴ˽ڵ�");
			return NULL;
		}
		else{
			//�����ǰ�ڵ�ֵ��key��.����ڵ㲻Ϊ��.ȥ�ҵ�ǰ�ڵ�����֧
			if(now_key>key&&p->leftchild!=NULL)
			{
				return addnode(p->leftchild,value,key,m+1);
			}
			
			//�����ǰ�ڵ�ֵ��key��.����ڵ�Ϊ��.��������ǰ����ڵ���
			if(now_key>key&&p->leftchild==NULL)
			{
				/**cout<<"�ڵ�key:"<<key<<"  ����㼶:"<<m<<endl;*/
				BTreeNode* i=new BTreeNode(value,key);
				BTreeNode* p_h=p->pre;
				p->pre=i;
				i->next=p;
				i->pre=p_h;
				if(p_h!=NULL)p_h->next=i;
				i->rightchild=p->leftchild;
				i->parent=p->parent;
				//�Ѹ��ڵ����һ����Ϊ�µĽڵ�
				if(p->parent!=NULL&&p->parent->leftchild==p){
					p->parent->leftchild=i;
				}
				else if(p->parent!=NULL&&p->parent->rightchild==p)
				{
					p->parent->rightchild=i;
				}
				//����Ǹ��ڵ�..ת��֮
				if(p==this->root)
				{
					this->root=i;
				}
				//����ڵ����ڲ�
				splitnode(i,m);
			}
			//�����ǰ�ڵ�ֵ��keyС.������֧��Ϊ��.ȥ�ҵ�ǰ�ڵ�����֧
			if(now_key<key&&p->rightchild!=NULL)
			{
				//��������������֧��key���ұ߽ڵ�keyС..ȥ�ұ߽ڵ�Ѱ��
				if(p->next!=NULL&&p->rightchild->key<p->next->key)
				{
					return addnode(p->next,value,key,m);
				}
				return addnode(p->rightchild,value,key,m+1);
			}
			
			//�����ǰ�ڵ�ֵ��keyС.������֧Ϊ��.ȥ�ҵ�ǰ�ڵ���
			if(now_key<key&&p->rightchild==NULL)
			{
				if(p->next!=NULL)
				{
					return addnode(p->next,value,key,m);
				}
				else
				{
					/**cout<<"�ڵ�key:"<<key<<"  ����㼶:"<<m<<endl;*/
					BTreeNode* i=new BTreeNode(value,key);
					p->next=i;
					i->pre=p;
					i->leftchild=p->rightchild;
					i->parent=p->parent;
					//����ڵ����ڲ�
					splitnode(i,m);
				}
			}
		
			
		}

		return p;
	}
BTreeNode* BTree::searchNode(int key)
	{
		return searchNode(this->root,key);
	}

BTreeNode* BTree::searchNode(BTreeNode* p,int key){
		//���p�ǿսڵ�..���ؿ�
		if(p==NULL)
		{
				return NULL;
		}
		int now_key=p->key;
		//������..�򷵻ؿ�
		if(now_key==key)
		{
			//throw exception("�Ѿ����ڴ˽ڵ�");
			return p;
		}
		else{
			//�����ǰ�ڵ�ֵ��key��.����ڵ㲻Ϊ��.ȥ�ҵ�ǰ�ڵ�����֧
			if(now_key>key&&p->leftchild!=NULL)
			{
				return searchNode(p->leftchild,key);
			}
			
			//�����ǰ�ڵ�ֵ��key��.����ڵ�Ϊ��.���ؿ�
			if(now_key>key&&p->leftchild==NULL)
			{
				return NULL;
			}
			//�����ǰ�ڵ�ֵ��keyС.������֧��Ϊ��.ȥ�ҵ�ǰ�ڵ�����֧
			if(now_key<key&&p->rightchild!=NULL)
			{
				//��������������֧��key���ұ߽ڵ�keyС..ȥ�ұ߽ڵ�Ѱ��
				if(p->next!=NULL&&p->rightchild->key<p->next->key)
				{
					return searchNode(p->next,key);
				}
				return searchNode(p->rightchild,key);
			}
			
			//�����ǰ�ڵ�ֵ��keyС.������֧Ϊ��.ȥ�ҵ�ǰ�ڵ���
			if(now_key<key&&p->rightchild==NULL)
			{
				if(p->next!=NULL)
				{
					return searchNode(p->next,key);
				}
				else
				{
					return NULL;
				}
			}
		
			
		}

		return p;
	}


int BTree::dump(ofstream* fout)
{
	return dump(fout,this->root);
}

int BTree::dump(ofstream* fout,BTreeNode* p){
		if(p==NULL) return 0;
			fout->write(p->value.c_str(),p->value.length());
			fout->write("\x0B",1);
			stringstream s;
			s<<p->key;
			fout->write(s.str().c_str(),s.str().length());
			fout->write("\x7E",1);
			if(p->pre==NULL)dump(fout,p->leftchild);
			dump(fout,p->rightchild);
			dump(fout,p->next);
		return 1;
	}

int BTree::load(ifstream* fin){

		char ch;

		stringstream s;

		string value="";

		string key="";

		int y=0;

		if(fin->fail())return 0;

		while(!fin->eof()) {
			ch = fin->get(); 
			s<<ch;
			if (ch == '\x0B')
			{
				//cout<<s.str()<<endl;
				//���s,,�����浽value
				value=s.str().substr(0,s.str().length()-1);
				s.str("");
			}
			if (ch == '\x7E')
			{
				y++;
				//cout<<s<<endl;
				key=s.str().substr(0,s.str().length()-1);
				this->insert(value,(int)atof(key.c_str()));
				//���s
				s.str("");
			}
		}
			
		return y;
	}

void BTree::clear()
	{
		//del(this->root);
		this->root=NULL;
	}

string BTree::search(BTreeNode* p,int key,string pit)
{
		if(p==NULL) return "\0";

		if(p->pre==NULL){

			pit=search(p->leftchild,key);

			if(pit!="\0")return pit;

		}
		pit=search(p->rightchild,key);

		if(pit!="\0")return pit;

		pit=search(p->next,key);

		if(pit!="\0")return pit;

		if(p->key==key)return p->value;

		return "\0";
	}

string BTree::tovector_le(BTreeNode* p,int max_key,int m){

	if(p==NULL)return " ";

	if(p->pre==NULL)tovector_le(p->leftchild,max_key,m+1);

	tovector_le(p->rightchild,max_key,m+1);

	tovector_le(p->next,max_key,m+1);

	if(p->key<max_key)
	{
		//cout<<p->key<<endl;

		stringstream ss;

		ss<<p->key;

		this->k.push_back(ss.str());

		ss.str("");
	}
	return " ";
}



int BTree::searchNode_le(int maxkey,BTreeNode* w,int dep){

	if(dep==0)k.clear();

	BTreeNode* p=NULL;

	//�������w�ڵ�(����һ���ݹ�������)�л���p�ڵ�
	if(w!=NULL)
	{
		p=w;
	}
	else
	{
		p=searchNode(this->root,maxkey);
	}
	//�ϼ��ڵ㲻Ϊ�����ϼ��ڵ��key����С
	if(p->parent!=NULL&&p->parent->key<maxkey)
	{
		return searchNode_le(maxkey,p->parent,1);

	}
	//ǰ��ڵ㲻Ϊ����ǰ��ڵ��key����С
	if(p->pre!=NULL&&p->pre->key<maxkey)
	{
		return searchNode_le(maxkey,p->pre,1);

	}
	//�ӽڵ㿪ʼ�����ǲ�Ľڵ�
	this->tovector_le(p,maxkey);

	return 1;
}


string BTree::tovector_gt(BTreeNode* p,int min_key,int m){

	if(p==NULL)return " ";

	if(p->pre==NULL)tovector_gt(p->leftchild,min_key,m+1);

	tovector_gt(p->rightchild,min_key,m+1);

	tovector_gt(p->next,min_key,m+1);

	if(p->key>min_key)
	{
		//cout<<p->key<<endl;

		stringstream ss;

		ss<<p->key;

		this->k.push_back(ss.str());

		ss.str("");
	}
	return " ";
}


int BTree::searchNode_gt(int minkey,BTreeNode* w,int dep){

	if(dep==0)k.clear();

	BTreeNode* p=NULL;

	//�������w�ڵ�(����һ���ݹ�������)�л���p�ڵ�
	if(w!=NULL)
	{
		p=w;
	}
	else
	{
		p=searchNode(this->root,minkey);
	}

	//�ϼ��ڵ㲻Ϊ�����ϼ��ڵ��key������
	if(p->parent!=NULL&&p->parent->key>minkey)
	{
		return searchNode_gt(minkey,p->parent,1);

	}
	//�ӽڵ㿪ʼ�����ǲ�Ľڵ�
	this->tovector_gt(p,minkey);

	return 1;
}


vector<string> BTree::searchNode_le(int maxkey,int dep){

	//������ʱ�ڵ�
	this->insert("NULL",maxkey);

	this->searchNode_le(maxkey,NULL,0);

	//ɾ����ʱ�ڵ�
	this->del(maxkey);

	return k;
}


vector<string> BTree::searchNode_gt(int minkey,int dep){

	//������ʱ�ڵ�
	this->insert("NULL",minkey);

	this->searchNode_gt(minkey,NULL,0);

	//ɾ����ʱ�ڵ�
	this->del(minkey);

	return k;
}