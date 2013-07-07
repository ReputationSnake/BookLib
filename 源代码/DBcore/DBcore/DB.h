//���ݿ����ͷ�ļ�
//
//by baizhongwei
//blog http://haku.webcpp.net
#ifndef DB_H
#define DB_H
#include "stdafx.h"
#include "BTreeM.h"
#include <direct.h>
namespace DBcore{
	class DB
	{
		friend ref class DBcore;
	private:
		//���ݿ��Ŀ¼
		string dbroot;
		//����
		string frmname;
		//B��������
		vector<BTree*> BTreeM;
		//�ֶ���
		vector<string> field;
		//�ֶ�����
		vector<string> fieldtype;
		//���ݿ���
		string dbname;
	protected:
		/**ɾ������
		*@param int Bpos
		*@param string value
		*/
		void delidx(int Bpos,string value);
		/**��value��s�ָ�
		*@param string value
		*@param char s
		*@return vector<string>
		*/
		vector<string> split(string value,char s);
		/**������
		*@param string db_name ���ݿ���
		*/
		void createfrm(string frm_name);
		/**�����ֶ�
		*@param string frmname ����
		*@vector<string> �ֶ���
		*/
		void createfield(string frmname,vector<string> tb);
		/**��Ѱ���
		*@param string frmname ����
		*@param string tdname �ֶ���
		*@param string value
		*@return vector<string>* �ҵ���ֵ��key
		*/
		vector<string>* select(string frmname,string tdname,string value,int y=-1);

		/**��Ѱ��maxvalueС�Ľ��
		*@param string frmname ����
		*@param string tdname �ֶ���
		*@param string maxvalue ���ֵ
		*@return vector<string>* �ҵ���ֵ��key
		*/
		vector<string>* selectls(string frmname,string tdname,string maxvalue,int y=-1);

		/**��Ѱ��maxvalueС�Ľ����ɾ��
		*@param string frmname ����
		*@param string tdname �ֶ���
		*@param string maxvalue ���ֵ
		*@return vector<string>* �ҵ���ֵ��key
		*/
		vector<string>* removels(string frmname,string tdname,string maxvalue,int y=-1);
		/**��Ѱ��minvalue��Ľ����ɾ��
		*@param string frmname ����
		*@param string tdname �ֶ���
		*@param string maxvalue ���ֵ
		*@return vector<string>* �ҵ���ֵ��key
		*/
		vector<string>* removegt(string frmname,string tdname,string minvalue,int y=-1);

		/**��Ѱ��minvalue��Ľ��
		*@param string frmname ����
		*@param string tdname �ֶ���
		*@param string maxvalue ���ֵ
		*@return vector<string>* �ҵ���ֵ��key
		*/
		vector<string>* selectgt(string frmname,string tdname,string minvalue,int y=-1);

		/**��result�ж�Ӧ��ɾ��
		*@param string frmname ����
		*@param string tdname �ֶ���
		*/
		void removeall(string frmname,string tdname,vector<string> valueAl);
		/**��Ѱ�������result��
		*@param string frmname ����
		*@param string tdname �ֶ���
		*@param string maxvalue ���ֵ
		*/
		vector<string>* selectall(string frmname,string tdname,vector<string> valueAl);
		/**��ȡ����
		*@param string index_filename �����ļ�
		*@param int i ���
		*/
		void loadindex(string index_filename,int i);
		/**���Ƿ����
		*@return bool
		*/
		bool ifexfrm();
		/**��ȡ�ֶ�
		*@param string frmname ����
		*@return bool
		*/
		bool loadfield(string frmname);
		/**�Ƿ�����ֶ�
		*@return bool
		*/
		bool ifexfield();
		/**ɾ����ֵ��ȵ�����
		*@param string frmname ����
		*@param string tdname �ֶ���
		*@param string value ֵ
		*/
		vector<string>* remove(string frmname,string tdname,string value,int y=-1);
		/**��������
		*@param string frmname ����
		*@vector<string> row �ֶ���
		*/
		void insert(string frmname,vector<string> row);
	public:
		/**��ʼ�����ڵ�
		*/
		DB(string dbroot);
			/**���������
			@access public
			*/
			vector<string>* result;
		/**������ִ�м򵥵ĵ���sql���
		*@param string queryline
		*/
		void queryline(string queryline);
	};


}
#endif