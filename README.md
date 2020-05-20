# README

## 1.Topic

實現數種Branch prediction(2 bit history predictor) :pushpin:

## 2.Input description

輸入BHT entry個數 換行  

輸入n個instruction，以-1當作結束  

:warning: 每一個instruction前面不能有空格，輸入範例如下圖一  

:warning: 若此instruction前面有label的話，label格式需為”L”+”數字”(此數字是阿拉伯數字不是中文數字)+”:”換行，下一行接instruction，所以label那行和接著的inst那行的address會相同(如下圖二，L1: beq…)  

:warning: 每個instruction要換行

![image](https://github.com/fup3su6/ALCO_project3/blob/master/input2.jpg)
![image](https://github.com/fup3su6/ALCO_project3/blob/master/input.jpg)

## 3.Output description

輸出該branch使用的entry、branch instruction 換行  

輸出state、預測值、實際值、misprediction數  

如下圖範例輸出

![image](https://github.com/fup3su6/ALCO_project3/blob/master/output.jpg)

## 4.Basic method

+ 利用while(getline){ }將每一行instrucion讀入存起來  
+ 先判斷是不是beq ( if yes )
    -  用vector<pair>把beq instrucion跟k(第幾個beq)存起來
	- PC mod BHT entry數量 計算每個beq instrucion 的 entry
+ 設prediction p並將每個entry的state初始化
+ 判斷如果有label就把label的PC存起來 
+ 值都存好後重新跑instructions，判斷 beq instrucion 將字串切割處理
+ 做出預測，並將state做出該有的變化印出


## 5.Code analysis

```c++
#include<iostream>
#include<string>
#include<vector>
#include<bitset>
using namespace std;
```
使用cin/cout，`include<iostream>`  

使用string取instruction，`include<string>`  

使用`vector`容器存branch  

會使用到2進位數字，`include<bitset>`來將10進位轉換成2進位

```c++
string state[4] = {"SN","WN","WT","ST" };
int Reg[32];
```
宣告`global string array`存可能的state  

宣告`global int`存register的value

```c++
struct pred{
	string n;
	int s[4];	
	int mis;
};
```
刻一個`struct`放predictor  

`string n` 表 **00/01/10/11**  

`int s[4]` 表示2*2=4個狀態，每個狀態裡存0/1/2/3，表 **SN/WN/WT/ST**  

`int mis` 存misprediction個數

```c++
int main() {
	int entries;
	cin >> entries;
```
宣告一個`int` 輸入`BHT entry個數`

```c++
	string inst[1000];
	int k = 0;		//第幾個beq,entry
	int entry[1000];
	int label[1000];
	pred p[1000];
	int lb = 1;
	typedef pair<string, int> par;
	vector<par> branch;

	string buffer;
	string str[1000];
	int pc = 0;	
	while (getline(cin, buffer)) {  //init
		if (buffer == "-1")
			break;
		if (buffer == "")
			continue;
		str[pc] = buffer;
		int pos;
		pos = buffer.find(" ");
		string x = buffer.substr(0, pos);
```
宣告一個`int pc`紀錄目前位置(表 *instrution address* )  

進入while迴圈 用`getline` `cin` 字串存進`buffer`如果為 **-1**則跳出  

將buffer(instrucions)存入`str陣列`  

宣告`int pos`紀錄當前inst內的 *第一個空格的位置*，再用`substr`方法將每行inst第一個空格前的字串存入`x`。ex: add、beq…(0到pos)

```c++
		if (x == "beq") {
			branch.push_back(par(buffer, k));
			inst[k] = buffer;
			entry[k] = pc % entries;
			for (int i = 0; i <= 3;i++)
			p[entry[k]].s[i] = 0;
			p[entry[k]].n = "00";
			p[entry[k]].mis = 0;
			k++;
		}
```
如果x為`beq`，將`pair(buffer(*instruction*), k(*第k個branch*))` **push.back**進入在while迴圈外宣告的`vector<pair>`型態的branch  

將buffer( *instruction* )放進在while迴圈外宣告的`inst array`(*存第k個branch的指令*)  

將`pc % entries`放進在while迴圈外宣告的`entry array`(*存第k個branch的entry*)  

Run 一個for迴圈，將第`entry[k]`的`p`裡的`s`**初始化**(*p為在while迴圈外宣告的pred struct型態的陣列，存對應entry的predict資訊*)  

再將第`entry[k]`的`p`裡的`n`、`mis`**初始化**

```c++
		pos = buffer.find(":");
		if (pos != -1) {
			label[lb] = pc;
			lb++;
			continue;
		}
		pc++;
	}//end while
```
讓pos為找到下一個`:`，如果`:`存在代表此行有label要記錄  
將`pc(address)`存入`label[lb]`(*label陣列存label的address，lb為第幾格label*)，pc++，結束while迴圈  

```c++
	for (int j = 0; j < pc;j++) {
		int pos;
		buffer = str[j];
		pos = buffer.find(" ");
		string x = buffer.substr(0, pos);
```
進入for迴圈，跑pc次(*pc=inst數量*)，同以上方法得到x(*每行inst第一個空格前的字串*)

```c++
		if (x == "beq") {
			pos = buffer.find("R");
			int pos2 = buffer.find(",");
			string m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r1 = stoi(m);

			buffer.erase(0, pos2 + 1);
			pos = buffer.find("R");
			pos2 = buffer.find(",");
			m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r2 = stoi(m);

			buffer.erase(0, pos2 + 1);
			m = buffer.substr(1, buffer.size());
			int la = stoi(m);
```
如果x為`beq`，用`pos`、`pos2`取第一個R後面的數字，再用`stoi`轉成`int`放入`r1`  
用`erase`將前面取過的擦掉，以同樣方法取第二個register數字放入`r2`，最後的數字放入`la`

```c++
			int idx;
			for (int i = 0;i < branch.size();i++) {
				if (branch[i].first == str[j]) {
					idx = branch[i].second;
					cout << "entry: " << entry[idx] << "　　　" << inst[idx] << endl;
					cout << "(" << p[entry[idx]].n << ", " << state[p[entry[idx]].s[0]] << ", " << state[p[entry[idx]].s[1]] << ", " << state[p[entry[idx]].s[2]] << ", " << state[p[entry[idx]].s[3]] << ")";
				}
			}
```
宣告一個`index(idx)`，代表第idx個branch  

用一個for迴圈跑完所有branch，如果branch pair的第一個(*instrucion*)找到對應的指令，將idx存入(*branch的第二個*)  

輸出對應`entry`的`instruction`、`state`

```c++
			bool pp = false, real = false;
			bitset<2> b(p[entry[idx]].n);
			unsigned long x = b.to_ulong();
			if (p[entry[idx]].s[x] == 0 || p[entry[idx]].s[x] == 1) {
				pp = false;
				cout << " N ";
			}
			else {
				pp = true;
				cout << " T ";
			}
```
宣告`bool pp`，表**預測值**，`bool real`表**實際值**  

宣告`2 bit` 的`bitset`將目前對應的`predictor`的`n`轉成2bit binary number存入x，如果`predictor`的`s[x] == 0 or 1`，表示目前`state`為 SN or WN  

預測值`pp`為*false*，輸出**Not taken**；else預測值`pp`為*true*，輸出**taken**

```c++
			if (Reg[r1] == Reg[r2]) {
				real = true;
				if (p[entry[idx]].s[x] < 3)
					p[entry[idx]].s[x]++;

				p[entry[idx]].n += "1";
				p[entry[idx]].n.erase(0, 1);

				j = label[la] - 1;
				if (real != pp)
					p[entry[idx]].mis++;
				cout << "T          misprediction:  " << p[entry[idx]].mis << endl;
				continue;
			}
```

如果`r1`在register裡的值**=**`r2`在register裡的值，表示`beq`會跳  

設實際值為*true*，且如果`s[x]!=3`，也就是**目前state不等於ST的話**，state往後一個  

目前`predictor`的`n`狀態+1(**n+”1”**，再`erase`掉最前面的)  

`設j = label[la] – 1`，讓下一次for的j++後j會等於`label[la]`，也就是*跳到label[la]的位置*  

如果**實際值!=預測值**，`misprediction++`  

輸出**taken**、**misprediction**數量

```c++
			else {
				real = false;
				if (p[entry[idx]].s[x] > 0)
					p[entry[idx]].s[x]--;

				p[entry[idx]].n += "0";
				p[entry[idx]].n.erase(0, 1);

				if (real != pp)
					p[entry[idx]].mis++;
				cout << "N          misprediction:  " << p[entry[idx]].mis << endl;
			}
		}//end if
```

如果`r1`在register裡的值**!=**`r2`在register裡的值，表示`beq`不會跳  

設實際值為*false*，且如果`s[x]!=0`，也就是**目前state不等於WN的話**，state往前一個  

目前`predictor`的`n`狀態+1(**n+”0”**，再`erase`掉最前面的)  

如果**實際值!=預測值**，`misprediction++`  

輸出**Not taken**、**misprediction**數量  

End if(x == beq)
```c++
		else if (x == "li") {
			pos = buffer.find("R");
			int pos2 = buffer.find(",");
			string m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r = stoi(m);
			buffer.erase(0, pos2 + 1);
			int value = stoi(buffer);
			Reg[r] = value;
		}
```

如果x為`li`，用`pos`、`pos2`取第一個R後面的數字，再用`stoi`轉成`int`放入`r`    

用`erase`將前面取過的擦掉，最後的數字放入`value`  

將`value`放入第`r`個`Reg`裡

```c++
		else if (x == "addi") {
			pos = buffer.find("R");
			int pos2 = buffer.find(",");
			string m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r1 = stoi(m);

			buffer.erase(0, pos2 + 1);
			pos = buffer.find("R");
			pos2 = buffer.find(",");
			m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r2 = stoi(m);

			buffer.erase(0, pos2 + 1);
			int value = stoi(buffer);

			Reg[r1] = Reg[r2] + value;
		}
```
如果x為`addi`，用`pos`、`pos2`取第一個R後面的數字，再用`stoi`轉成`int`放入`r1`  

用`erase`將前面取過的擦掉，以同樣方法取第二個register數字放入`r2`，最後的數字放入`value`  

將`value`加上第`r2`個`Reg`，再放入第`r1`個`Reg`裡

```c++
		else if (x == "add") {
			pos = buffer.find("R");
			int pos2 = buffer.find(",");
			string m = buffer.substr(pos + 1, pos2 - pos - 1);
			int rd = stoi(m);

			buffer.erase(0, pos2 + 1);
			pos = buffer.find("R");
			pos2 = buffer.find(",");
			m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r1 = stoi(m);

			buffer.erase(0, pos2 + 1);
			m = buffer.substr(1, buffer.size());
			int r2 = stoi(m);

			Reg[rd] = Reg[r1] + Reg[r2];
		}
	}//end for
}//end main
```

如果x為`add`，用`pos`、`pos2`取第一個R後面的數字，再用`stoi`轉成`int`放入`rd`  

用`erase`將前面取過的擦掉，以同樣方法取第二個register數字放入`r1`，最後的數字放入`r2`  

將第`r1`個`Reg`加上第`r2`個`Reg`，再放入第`rd`個`Reg`裡  

End for  

End main


