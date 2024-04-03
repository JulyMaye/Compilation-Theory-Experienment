#include <iostream>
#include <fstream>
#include <string>
#include<set>
#include< iomanip >
//在 C++ 中，setw 是 <iomanip> 头文件中定义的一个函数，用于设置输出流的字段宽度（field width）。
//setw 函数接受一个整数参数，该参数指定了下一个输出字段的最小宽度。
//当你使用 setw(n) 时，它会影响接下来的输出，使得输出的字符串至少占据 n 个字符的宽度，不够的话会在输出字符串前面填充空格。
using namespace std;
set<string>keyword = { "asm", "do", "if", "return", "typedef", "auto", "double", "inline", "short", "typeid", "bool", "dynamic_cast", "int", "signed", "typename", "break", "else", "long", "sizeof", "union", "case", "enum", "mutable", "static", "unsigned", "catch", "explicit", "namespace",
"static_cast", "using", "char", "export", "new", "struct", "virtual", "class", "extern", "operator", "switch", "void", "const", "false", "private","include",
"template", "volatile", "const_cast", "float", "protected", "this", "wchar_t", "continue", "for", "public", "throw", "while", "default", "friend",
"register", "true", "delete", "goto", "reinterpret_cast", "try","stdio.h","main","printf"};
set<string>arithOperator = { "+", "-", "*", "%", "/", "+=", "-=","++","--"};
set<string>logicOperator = { "&&", "||", "!", "&", "^", "|", "~" };
set<string>relationOperator = { "<", "<=", "=", "==", ">", ">=", "!=" };
set<string>delimiter = { ";", ",", "(", ")", "{", "}" };
set<string>special = { "#","//","/*" };

//判断是否是字母
bool isLetter(char var) {
    //强制转换
    int ascii = (int)var;
    bool bo = ((ascii >= 'A') && (ascii <= 'Z')) || ((ascii >= 'a') && (ascii <= 'z'));
    if (bo) {
        return true;
    }
    else {
        return false;
    }
}
//判断字符是否为数字
bool isDigit(char var) {
    //强制转换
    int ascii = (int)var;
    bool bo = ((ascii >= '0') && (ascii <= '9'));
    if (bo) {
        return true;
    }
    else {
        return false;
    }
}
//判断一个字符是否为分割符
bool isBoundary(char var) {
    //不是字母、数字、下划线、小数点可认为是分割符
    if ((!isLetter(var)) && (!isDigit(var)) && (var != '_') && (var != '.') && (var != '$') && (var != '@') && (var != '#') && (var != '&')) {
        return true;
    }
    return false;
}
//检测一个字符是否为分界符
bool isDelimiter(char var) {
    string charAsString(1, var);//将字符转为字符串

    // 检查字符是否在delimiter中
    if (delimiter.find(charAsString) != delimiter.end()) {
        return true;
    }
    else {
        return false;
    }
}
//预处理，将注释替换成空格
void processFile(ifstream& inFile, ofstream& outFile) {
    string line;
    int count = 0;
    while (getline(inFile, line)) {
        size_t pos;
        //npos 的值通常是一个非常大的整数，表示在字符串中未找到指定的子字符串或字符时的返回值。
        while ((pos = line.find("//")) != string::npos) {
            
            if (pos != string::npos) {
                line.replace(pos, string::npos, line.size() - pos, ' '); // 用空格替换/*和*/之间的内容
                
            }
            else {
                // 如果没有找到匹配的*/，则提示错误或者做相应处理
                cout << "Error: // without matching //" << std::endl;
                return;
            }
        }
        while ((pos = line.find("/*")) != string::npos) {
            size_t end_pos = line.find("*/", pos);
            if (end_pos != string::npos) {
                line.replace(pos, end_pos - pos + 2, end_pos - pos + 2, ' '); // 用空格替换/*和*/之间的内容,第三个参数表示重复次数
                for (int i = 0; i < count; i++) {
                    line += '\n';
                }
            }
            else {
                    // 如果没有找到匹配的*/，则提示错误或者做相应处理
                    if (inFile.eof()) {
                        cout << "Error: /* without matching */" << std::endl;
                        return;
                    }
                    // 如果未到文件结尾，继续读取下一行进行匹配
                    string next_line;
                    if (!getline(inFile, next_line)) {
                        cout << "Error: Unable to read next line" << endl;
                        return;
                    }
                    else
                    {
                        count++;
                        line += next_line;
                    }
                }                    
            
        }
        outFile << line << endl;
    }
    return;
}
void process(ifstream& file, ofstream& resultFile)
{
    if (resultFile.is_open()) {
        if (file.is_open()) {
            string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); // 一次性读取文件内容到字符串
            int i = 0, row = 0, column = 0;
            while (i != content.size()) {
                char currentChar = content[i];
                string charAsString(1, currentChar);//将字符转为字符串
                // 在这里进行字符的逐个分析
                if (currentChar == '\n')
                {
                    row++;
                    column = 0;
                }
                if (currentChar == '#')
                {
                    cout << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "预处理符号" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                    resultFile << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "预处理符号" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;

                    i++;
                    column++;
                }
                else if (isLetter(currentChar))//是字母
                {
                    int j = i;
                    bool flag = false;//看是否是非法字符（标识符只能由字母数字下划线组成）
                    while (!isBoundary(content[j]))
                    {
                        if (content[j] != '_' || !isLetter(content[j]) || !isDigit(content[j]))//出现非法字符
                            flag = true;
                        j++;
                        column++;
                    }
                    column -= 1;
                    string tempString = content.substr(i, (j - i));
                    if (keyword.find(tempString) != keyword.end()) {
                        cout << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "关键字" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        resultFile << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "关键字" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;

                    }
                    else if(flag){
                        cout << setw(20) << tempString << setw(20) << (string)("(标识符," + tempString + ")") << setw(20) << "标识符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        resultFile << setw(20) << tempString << setw(20) << (string)("(标识符," + tempString + ")") << setw(20) << "标识符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;

                    }
                    else
                    {
                        cout << setw(20) << tempString << setw(20) << (string)("(Error," + tempString + ")") << setw(20) << "Error" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        resultFile << setw(20) << tempString << setw(20) << (string)("(Error," + tempString + ")") << setw(20) << "Error" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                    }
                    i = j;
                }
                else if (isDigit(currentChar))//是数字
                {
                    int j = i;
                    bool flag = false;//判断是否为Error字符
                    int flag_point = 0;//判断小数点出现的次数是否超过一次
                    while (!isBoundary(content[j]))
                    {
                        if (!isDigit(content[j]) && (content[j] != '.'))
                        {
                            flag = true;//数字之间出现了非小数点的字符
                        }
                        if (content[j] == '.')
                        {
                            flag_point ++;//小数点个数超过一个也是非法字符
                        }
                        j++;
                        column++;

                    }
                    column -= 1;
                    string tempString = content.substr(i, (j - i));
                    if (flag||flag_point>1) {
                        cout << setw(20) << tempString << setw(20) << (string)("(Error," + tempString + ")") << setw(20) << "Error" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        resultFile << setw(20) << tempString << setw(20) << (string)("(Error," + tempString + ")") << setw(20) << "Error" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;

                    }
                    else {
                        cout << setw(20) << tempString << setw(20) << (string)("(常数," + tempString + ")") << setw(20) << "常数" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        resultFile << setw(20) << tempString << setw(20) << (string)("(常数," + tempString + ")") << setw(20) << "常数" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                    }
                    i = j;
                }
                else//既不是数字又不是字母
                {

                    string tempString = content.substr(i, 2);
                    if (isDelimiter(currentChar))
                    {
                        cout << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "分界符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        resultFile << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "分界符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;

                        i++;
                        column++;
                    }
                    // 检查字符是否属于运算符
                    else  if (arithOperator.find(charAsString) != arithOperator.end()) {
                        if (arithOperator.find(tempString) != arithOperator.end()) {//由于算术运算符可能占据两个字符，所以需要超前搜索进一步判断
                            cout << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "算术运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            resultFile << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "算术运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            i += 2;
                            column += 2;
                        }
                        else
                        {
                            cout << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "算术运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            resultFile << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "算术运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            i++;
                            column++;
                        }
                    }
                    else if (logicOperator.find(charAsString) != logicOperator.end())
                    {
                        if (logicOperator.find(tempString) != logicOperator.end())//由于位运算符可能占据两个字符，所以需要超前搜索进一步判断
                        {
                            cout << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "位运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            resultFile << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "位运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            i += 2;
                            column += 2;
                        }
                        else
                        {
                            cout << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "位运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            resultFile << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "位运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            i++;
                            column++;
                        }
                    }
                    else if (relationOperator.find(charAsString) != relationOperator.end())
                    {
                        if (relationOperator.find(tempString) != relationOperator.end())//由于关系运算符可能占据两个字符，所以需要超前搜索进一步判断
                        {
                            cout << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "关系运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            resultFile << setw(20) << tempString << setw(20) << (string)("(" + tempString + ",-)") << setw(20) << "关系运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            i += 2;
                            column += 2;
                        }
                        else
                        {
                            cout << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "关系运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            resultFile << setw(20) << currentChar << setw(20) << (string)("(" + charAsString + ",-)") << setw(20) << "关系运算符" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                            i++;
                            column++;
                        }
                    }
                    else if (isBoundary(currentChar))
                    {
                        i++;
                        column++;
                    }
                    else
                    {
                        int j = i;
                        bool flag = false;
                        while (!isBoundary(content[j]))
                        {
                            j++;
                            column++;
                        }
                        column -= 1;
                        cout << setw(20) << tempString << setw(20) << (string)("(Error," + tempString + ")") << setw(20) << "Error" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        resultFile << setw(20) << tempString << setw(20) << (string)("(Error," + tempString + ")") << setw(20) << "Error" << setw(20) << "(" << (row + 1) << "," << (column + 1) << ")" << endl;
                        i = j;
                    }
                }
            }

            file.close(); // 关闭文件
        }
        else {
            cout << "无法打开文件" << endl;
        }
        resultFile.close();
    }
    else {
        cout << "无法打开文件" << std::endl;
    }
}
int main() {
    ofstream outFile("modified_code.txt"); // 打开名为 code.txt 的文件
    cout << setw(20) << "单词" << setw(20) << "二元序列" << setw(20) << "类 型" << setw(30) << "位置（行，列）" << endl;
    ifstream inFile("code.txt");
    if (!outFile) {
        cout << "Error: Unable to open output file!" << endl;
        return 1;
    }
    processFile(inFile, outFile);
    ifstream file("modified_code.txt");//预处理后的文件
    ofstream resultFile("result.txt"); // 打开文件以进行追加写入,ios_base是输入输出流基类，app(append)是指示在文件末尾追加写入内容的标志。
    process(file, resultFile);
	return 0;
}
