
#include "qe.h"

Filter::Filter(Iterator* input, const Condition &condition) 
{
    RecordBasedFileManager *rbfm = RecordBasedFileManager::instance();
    RelationManager *rm = RelationManager::instance();
    RBFM_ScanIterator iter;
    FileHandle fileHandle;

    char arr[condition.lhsAttr.length()];
    strcpy(arr, condition.lhsAttr.c_str());
    string tableName = "";
    string conditionAttribute="";
    int flag =0;
    for(int i=0;i<sizeof(arr);i++)
    {
        if(arr[i]=='.')
        {    
            flag=1;
            continue;
        }
        if(flag==0)
            tableName+=arr[i];
        else
            conditionAttribute+=arr[i];

    }
    vector<Attribute> attrs;
    int openFileSuccess = rbfm->openFile(tableName, fileHandle);
    int getAttributeSucess = rm->getAttributes(tableName, attrs);
    vector<string> attributeNames;
    if(openFileSuccess==0 && getAttributeSucess==0)
    {
        rbfm->scan(fileHandle, attrs, conditionAttribute ,condition.op, &condition.rhsValue, attributeNames , iter);
    }
}

Project::Project(Iterator *input, const vector<string> &attrNames)
{
    RecordBasedFileManager *rbfm = RecordBasedFileManager::instance();
    RelationManager *rm = RelationManager::instance();
    RBFM_ScanIterator iter;
    FileHandle fileHandle;
    string rawValue = attrNames.front();
    char arr[rawValue.length()];
    strcpy(arr, rawValue.c_str());
    string tableName = "";
    int attrNamesSize = attrNames.size();
    for(int i=0;i<sizeof(arr);i++)
    {
        if(arr[i]=='.')
        {   
            break;
        }
        tableName+=arr[i];
    }
    vector<string> attributes;
    int flag;
    for(int i=0;i<attrNamesSize;i++)
    {
        string attrName = attrNames.at(i);
        char arr[attrName.length()];
        strcpy(arr,attrName.c_str());
        flag = 0;
        string attr="";
        for(int i=0;i<sizeof(arr);i++)
        {
            if(arr[i]=='.')
            {   
                flag = 1;
                continue;
            }
            if(flag)
                attr+=arr[i];
        }
        attributes.push_back(attr);         
    }
    vector<Attribute> attrs;
    int openFileSuccess = rbfm->openFile(tableName, fileHandle);
    int getAttributeSucess = rm->getAttributes(tableName, attrs);
    if(openFileSuccess==0 && getAttributeSucess==0)
    {
        rbfm->scan(fileHandle, attrs, NULL ,NO_OP,NULL, attributes , iter);
    }
}

