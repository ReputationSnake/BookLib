#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;
#include "DB.h"

namespace DBcore {

	/// <summary>
	/// CCDControl ժҪ
	/// </summary>
	///
	/// ����: ������Ĵ�������ƣ�����Ҫ����
	///          ����������������� .resx �ļ��������й���Դ���������ߵ�
	///          ����Դ�ļ��������ԡ�����
	///          �������������˴���Ĺ���
	///          ���ػ���Դ��ȷ������
	public ref class DBcore : public System::Windows::Forms::UserControl
	{
	public:
		DBcore(String^ dbroot)
		{
			d=new DB(ConvertToString(dbroot));
			line=new vector<string>();
		}
		void query(String^ query)
		{
			try
			{
				d->result->clear();
				d->queryline(ConvertToString(query));
				nowline=0;
				totalnums=d->result->size();
			}
			catch (Exception^ e)
			{
				throw e;
			}
		}
		/**������������
		 *ÿ����һ������������һ��
		 */
		bool fetchline()
		{
			if(d->field.size()<=0) return false;
			int i=0,y=0;
			line->clear();
			string tmp="";
			while(y<(int)(totalnums/d->field.size()))
			{
				for (i=0;i<(int)d->field.size();i++)
				{
					tmp=d->result->at(y*d->field.size()+i);
					if(y==nowline)
					{
						line->push_back(tmp);
					}
				}
				if(y==nowline)
				{
					nowline++;
					break;
				}
				y++;
			}
			
			if(line->size()==0)
				return false;
			return true;

		}
		/**�õ���ǰ��ʱ��ֵ
		 *@param String^ tablename
		 */
		String^ get(String^ tablename)
		{
			if(line->size()==0)
				return "";
			int i=0;
			int tablepos=-1;
			//�õ����Ӧtablepos
			for(i=0;i<(int)d->field.size();i++){
				if(d->field[i]==ConvertToString(tablename))
				{
					tablepos=i;
					break;
				}
			}
			if(tablepos==-1)
			{
				cout<<"error:����ı���"<<endl;
				return "";
			}

			return gcnew String(line->at(tablepos).c_str());
		}
	private:
		DB* d;
		vector<string>* line;
		int nowline;
		int totalnums;
	protected:
		std::string ConvertToString(System::String^ str)
		{
			int q=(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
			char* p=(char*)q;
			return std::string(p);
		}
	};
}
