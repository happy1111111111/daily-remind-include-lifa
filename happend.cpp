/*说明 :

1,函数
	int week 输入日期判定星期几 
	int strtoint 转换str->int 
	str input_ymd 输入日期并转换为yyyymmdd 检查合法性
	void calcday 计算加若干天后的日期 
	int kacha 删除数据 结束后没有刷新et 
	int daojishi 距离指定日期还有几天 超过为负值 如 超过10086天输出-10086 
2,文件格式
	文件名 daily  C:\\Users\\xck\\Desktop\\TIME\\daily
	(1)yyyymmdd(下次理发日期)
	(2)yyyymmdd事件
	(3)yyyymmdd事件
	...... 
3,操作
	退出[1]
	增加日程[2]
	删除日程[3]
	查询文件内所有日程[4]
	删除30天以前的数据[5]
	刷新并且重新输出[6]
	计算间隔日期[7]
		知道起日期和间隔计算讫日期[1]
		知道起讫日期计算间隔[2]
	更新理发信息[8]
		自动输入当前时间[1]
		手动输入[2]
	关于本程序[9]
	清屏[0]
4,署名及版权 查看 关于本程序[9]
5,最多500个日程 
*/
//自动删除30天以前的
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <windows.h>
#include <cmath>
#include <sstream> 

const char* fn = "C:\\Users\\qaz123\\Desktop\\xck\\tools\\TIME\\daily";

void MaximizeConsole() {//最大化 
    HWND hwnd = GetForegroundWindow();
    ShowWindow(hwnd, SW_MAXIMIZE);
}

struct eventtime{
	int year,month,day,num;
	std::string event;
	bool info;//真为删除 
}et[500];

int lsy,lsm,lsd;
std::string lsz;
bool firstline = false;

int week(int y,int m,int d){//输入日期判定星期几 
	int w;
	if(y == 0 ||m == 0 || d == 0) {
		return -1;
	}
    if(m == 1){
        y--;
        m = 13;
    }
    if(m == 2){
        y--;
        m = 14;
    }
	w = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
	w++;
	return w;
}

int strtoint(std::string a){//str->int 
	int len = a.size(),b = 0;
	for(int i = 0;i < len;i++){
		b *= 10;
		b = b + a[i] - '0';
	}
	return b;
}

std::string input_ymd(){//输入年月日 
	std::string y,m,d,data="";
	std::cout<<"按照以下样例输入 "<<"不足指定位数的会自动补零"<<std::endl;
	std::cout<<"        yyyy mm rr"<<std::endl<<"input>> ";
	std::cin>>y>>m>>d;
	while(y.size() > 4 ||m.size() > 2 || d.size() > 2 ||strtoint(m) > 12||strtoint(m) < 0||strtoint(d) < 0|| strtoint(d) > 31){
		std::cout<<"输入非法 检查后重新输入"<<std::endl;
		std::cout<<"        yyyy mm rr"<<std::endl<<"input>> ";
		std::cin>>y>>m>>d;
	}
	for(int i = y.size();i < 4;i++) data +='0';
	data += y;
	for(int i = m.size();i < 2;i++) data +='0';
	data += m;
	for(int i = d.size();i < 2;i++) data +='0';
	data += d;
	return data;
}

void calcday(int ning,int yue,int ri,int n){//计算加若干天后的日期 
	//             01 02 03 04 05 06 07 08 09 10 11 12
	int day[] = {1,31,28,31,30,31,30,31,31,30,31,30,31};
	int x = 1;//加天数还是减天数
	if(n < 0){
		x = -1;
	}
	n = abs(n);
	for(int i = 0;i < n;i++){
		if((ning % 4 == 0 && ning % 100 != 0) || (ning % 100 == 0 && ning % 400 == 0)){//闰年 
			day[2] = 29;
		}else{
			day[2] = 28;
		}
		ri += x;
		//接下来处理进退位 
		if(x == 1){//进位 
			if(ri > day[yue]){
				yue++;
				if(yue > 12){
					yue = 1;
					ning++;
				}
				ri = 1;
			}
		}else if(x == -1){//退位 
			if(ri == 0){
				yue--;
				if(yue == 0){
					yue = 12;
					ning--;
				}
				ri = day[yue];
			}
		}
	}
	lsy=ning;
	lsm=yue;
	lsd=ri;
}

void kacha(int a){//删除数据 结束后没有刷新 
//初始化 
	for(int i = 0;i < 500;i++){
		et[i].info = false;
	}
	//读取文件
	std::ifstream inFile(fn);  // 创建输入文件流对象
	if (!inFile.is_open()) {
        std::cerr << "无法打开文件: " << fn << std::endl;
        system("pause");
        return ;
    }
//逐行读入ls
    std::string line,ls[501];
    int etcount = 0;
    firstline = false;
    while (std::getline(inFile, line)) {
    	if(firstline == false){
    		ls[500] = line;
    		firstline = true;
    		continue;
		}
		if(line == ""){
			continue;
		}
        etcount++;
		ls[etcount] = line; 
        if (etcount == a){
        	et[etcount].info = true;
		}
    }
    firstline = false;
    inFile.close();
    std::ofstream outFile(fn); // 创建输出文件流对象
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件进行写入: " << fn << std::endl;
   	}
   	outFile <<ls[500]<<"\n";//写入理发数据(特判 
	for(int i = 1;i <= etcount;i++){
   		if(et[i].info != true){
   				outFile <<ls[i]<<"\n";
		}
	}
	outFile.close(); // 关闭文件
}

int daojishi(int f_day,int f_momth,int f_year,int s_day,int s_momth,int s_year){//处理倒计时
	//判断f_xxx大还是s_xxx大
	bool symbol = false;//假 => f_xxx < s_xxx => 符号为 + |真 => f_xxx > s_xxx  => 符号为 - 
	if((f_year > s_year) || ((f_year == s_year) && (f_momth > s_momth)) || ((f_year == s_year) && (f_momth == s_momth) && (f_day > s_day))){
		int djs1;
		symbol = true;
		//交换年 
		djs1 = s_year;
		s_year = f_year;
		f_year = djs1;
		//交换月 
		djs1 = s_momth;
		s_momth = f_momth;
		f_momth = djs1;
		//交换日 
		djs1 = s_day;
		s_day = f_day;
		f_day = djs1;
	}
	int day[] = {1,31,28,31,30,31,30,31,31,30,31,30,31};
	for(int i = 0;;i++){
		if(f_year == s_year && f_momth == s_momth && f_day == s_day){
			if(symbol){
				i *= -1;
			}
			return i;
		}
		if((f_year % 4 == 0 && f_year % 100 != 0) || (f_year % 100 == 0 && f_year % 400 == 0)){//闰年 
			day[2] = 29;
		}else{
			day[2] = 28;
		}
		f_day += 1;
		if(f_day > day[f_momth]){
			f_momth++;
			if(f_momth > 12){
				f_momth = 1;
				f_year++;
			}
			f_day = 1;
		}
	}
}

//MAINMAINMAINMAIN

int main() {
	Sleep(1000);
	std::cout<<"欢迎: HatsuneMikuIsMyWife@061031xck"<<std::endl;
//初始化 
	for(int i = 0;i < 500;i++){
		et[i].year = -2;
		et[i].month = -2;
		et[i].day = -2;
		et[i].num = -2;
	}
	//窗口最大化 
    //MaximizeConsole();
//显示miku,系统时间 
	printf("             *#^##^*                                                                                 \n"); 
	printf("            *########*                                                                               \n"); 
	printf("           #### ! <)##                                                                               \n"); 
	printf("          #####    #####                                                                        	 \n"); 
	printf("         ###   |##r  ####                      _##______      ___##___        #______      _______   \n"); 
	printf("        *### /:.#:#.   ###                    |__  /___ |     |________|     |_______|    / _____  | \n"); 
	printf("       *#&#  d.:(@}{@)  ####                    / / || ||    ___||__||___      ________   | /    / / \n"); 
	printf("      *####  01 #######  ####                  / /  || ||   |____________|   |________|   |/    / /  \n"); 
	printf("     *####   ##I #######  #####               / /|| || ||     |________|       ________        / /   \n"); 
	printf("    *#%###   ##;  :#####:   ##$##            /_/||  || ||     |______| |      |______  |      / /    \n"); 
	printf("   *#####   /##   ($=[]=$)   ######             ||  || ||     |________|             | |     / /     \n"); 
	printf("  /*##$#*   ###  /%$&###&$%    ###$##         H A T S U N E  M I K U 16p with 项宸恺 |_|    /_/      \n"); 
	printf("  #####/   *### /%$########$%   ######                                                               \n"); 
	printf(" *#####   /####/%$##########$%  *###$##                                                              \n"); 
	printf("*######   ##### %#############%  *#####                                                              \n"); 
	printf("*##&### .#######|wom#wom#wom#wo#* *#&###                                                             \n"); 
	printf("*#####* .##=#=#/_I_### ###_I_##/   *#####                                                            \n"); 
	printf("*#####/   (%:  * - -*    *- -*      #####                                                            \n"); 
	printf("*#####*        :  |)      |  )     *#%$#                                                             \n"); 
	printf("(%#%#*         -H#/        XH0     /:.#                                                              \n"); 
	printf(" （$.%#*        =##        ###(     #*#                                                              \n"); 
	printf("  (#:.:#      ###         l#d       /                                                                \n"); 
	printf("    (*/      (#}           {#)                                                                       \n"); 
	printf("            /#/            :##                                                                       \n"); 
	printf("           /#/              ##:                                                                      \n"); 
	printf("           ##               ##                                                                       \n"); 
	printf("          /-=              /%=                                                                       \n"); 
	printf("         (=$#              ##$:                                                                      \n"); 
	printf("         |3__)            9__/                                                                     \n\n");  
	SYSTEMTIME time;
	GetLocalTime(&time);
	printf("当前时间(UTC/GMT +8.00 (东八区)): %d年%d月%d日  %d时%d分%d秒%d毫秒  星期",time.wYear,time.wMonth
		,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	if(time.wDayOfWeek == 0){//星期天 
		 std::cout<<"天"<<std::endl; 
	}else{
		std::cout<<time.wDayOfWeek<<std::endl;
	}
//读取文件
	int qdcount = 0;
	std::ifstream inFile(fn);  // 创建输入文件流对象
	while(!inFile.is_open()) {
		qdcount += 1;
        Sleep(10000);
        std::cout<<qdcount<<std::endl;
        std::ifstream inFile(fn);
        if(qdcount > 10){
        	std::cerr << "无法打开文件: " << fn << std::endl;
        	system("pause");
        	return 1;
		}
    }
//逐行读入结构体 
    std::string line;
    int etcount = 0;
    firstline = false;//安全 + 第一
    while (std::getline(inFile, line)) {
    	if(firstline == false){//特判第一行  直接把理发信息输出掉 
    		firstline = true;
    		std::cout<<std::endl<<"=>下次理发时间为: "<<strtoint(line.substr(0, 4))<<" 年 "<<strtoint(line.substr(4, 2))<<" 月 "<<strtoint(line.substr(6, 2))<<" 日 星期 "<<
				week(strtoint(line.substr(0, 4)),strtoint(line.substr(4, 2)),strtoint(line.substr(6, 2)))<<" , 还有 ["<<
				daojishi(time.wDay,time.wMonth,time.wYear,strtoint(line.substr(6, 2)),strtoint(line.substr(4, 2)),strtoint(line.substr(0, 4)))<<"] 天"<<std::endl<<std::endl;
			continue;
		}
		if(line == ""){
			continue;
		}
    	//       0123456789
    	//格式为:yyyymmddXXXXXXXXXXXXXXXX 
        etcount++;
        et[etcount].year = strtoint(line.substr(0, 4));//substr截取子串 
        et[etcount].month = strtoint(line.substr(4, 2));
        et[etcount].day = strtoint(line.substr(6, 2));
        et[etcount].event = line.substr(8);
        et[etcount].num = etcount;
    }
    firstline = false;
    inFile.close();
//处理日程信息 
    int thatmonth = 0,thatday = 0;
	int th[500],td[500];
	for(int i = 0;i < etcount + 1;i++){
		if(et[i].year > -2){
			if(et[i].year == 0 || et[i].year == time.wYear){
				if(et[i].month == 0 || et[i].month == time.wMonth){
					th[thatmonth] = i;
					thatmonth++;
					if(et[i].day == 0 ||et[i].day == time.wDay){
						td[thatday] = i;
						thatday++;
					}
				}
			}
		}
	}
	 
//输出日程信息 
	std::cout<<"文件内共计 "<<etcount + 1<<" 件日程"<<std::endl;
	std::cout<<std::endl<<"本月事件共有 "<<thatmonth<<" 件:"<<std::endl;
	for(int i = 0;i < thatmonth;i++){
		std::cout<<"    "<<i + 1<<" : "<<et[th[i]].month<<"/"<<et[th[i]].day<<" "<<et[th[i]].event<<std::endl; 
	}
	if(thatday > 0){
		std::cout<<"▆▆▆▆▆▆▆▆▆▆▆"<<std::endl; 
	}
	std::cout<<std::endl<<"本日事件共有 "<<thatday<<" 件:"<<std::endl;
	for(int i = 0;i < thatday;i++){
		std::cout<<"    "<<i + 1<<" : "<<et[td[i]].event<<"▆"<<std::endl; 
	}
	


//循环处理增删查
	std::string info;
	std::cout<<std::endl;
	int order;
	while(1){
		std::cout<<"退出[1] 增加日程[2] 删除日程[3] 查询文件内所有日程[4] 删除30天以前的数据[5] 刷新并且重新输出[6] 计算间隔日期[7] 更新理发信息[8] 关于本程序[9] 清屏[0]"<<std::endl<<"input>> ";
		std::cin>>order;//未对字符进行检查! 
		switch(order){
			
			case 0:{//清屏 
				system("cls");
				printf("当前时间(UTC/GMT +8.00 (东八区)): %d年%d月%d日  %d时%d分%d秒%d毫秒  星期",time.wYear,time.wMonth
					,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
				if(time.wDayOfWeek == 0){//星期天 
		 			std::cout<<"天"<<std::endl; 
				}else{
					std::cout<<time.wDayOfWeek<<std::endl;
				}
        		break;
			}
			
    		case 1:{//退出 
    			return 0;
			}
			
    		case 2:{//增加 
    			//打开文件 
    			std::ofstream outFile(fn,std::ios::app); // 创建输出文件流对象
    			if (!outFile.is_open()) {
        			std::cerr << "无法打开文件进行写入: " << fn << std::endl;
        			break;
   				 }
   				 //input 
    			info = "n";
    			std::string events;
    			std::string inymd;
    			while(info[0] == 'n'){
    				std::cout<<"如果是每年这个时候都要做的事情，年份位就赋值为0,月份，日同理。"<<std::endl;
    				inymd = input_ymd();
    				std::cout<<"日期正确吗(y/n): "<<inymd<<" 星期"<<week(strtoint(inymd.substr(0, 4)),strtoint(inymd.substr(4, 2)),strtoint(inymd.substr(6, 2)))<<std::endl<<"input>> ";
					std::cin>>info; 
				}
				std::cout<<"输入事件:"<<std::endl<<"input>> ";
				std::cin>>events;
				outFile <<inymd<<events<<"\n";
				outFile.close(); // 关闭文件
				std::cout<<"已经写入: "<<inymd<<events<<std::endl;
    			break;
			}
			
    		case 3:{//删除日程
    			//查询所有日程 
    			//读取文件
				std::ifstream inFile(fn);  // 创建输入文件流对象
				if (!inFile.is_open()) {
    			    std::cerr << "无法打开文件: " << fn << std::endl;
    		    	break; 
    			}
				//逐行输出 
				std::cout<<"   0123456789"<<std::endl; 
				std::cout<<"n: yyyymmddXXXXXXXXXXXXXXXX"<<std::endl; 
				int count = 0;
				firstline = false;
    			while (std::getline(inFile, line)) {
    				if(firstline == false){
    					firstline = true;
    					continue;
					}
					if(line == ""){
						continue;
					}
    				//       0123456789
    				//格式为:yyyymmddXXXXXXXXXXXXXXXX 
					count++; 
        			std::cout<<count<<": "<<line<<std::endl;
    			}
    			firstline = false;
    			std::cout<<"共计 "<<count<<" 项日程"<<std::endl;
    			inFile.close();
    			std::cout<<"录入要删除的日程编号"<<std::endl<<"input>> ";
    			int a;
    			std::cin>>a;
    			while(a > count){
    				std::cout<<"非法输入 请重新输入"<<std::endl<<"input>> ";
    				std::cin>>a;
				}
    			kacha(a);
    			std::cout<<"删除操作完成"<<std::endl;
				break;
			}
			
			case 4:{//查询所有日程
				//读取文件
				std::ifstream inFile(fn);  // 创建输入文件流对象
				if (!inFile.is_open()) {
    			    std::cerr << "无法打开文件: " << fn << std::endl;
    		    	break; 
    			}
				//逐行输出 
				std::cout<<"   0123456789"<<std::endl; 
				std::cout<<"n: yyyymmddXXXXXXXXXXXXXXXX"<<std::endl; 
				int count = 0;
				firstline = false;
    			while (std::getline(inFile, line)) {
    				if(firstline == false){
    					firstline = true;
    					continue;
					}
					if(line == ""){
						continue;
					}
    				//       0123456789
    				//格式为:yyyymmddXXXXXXXXXXXXXXXX 
					count++; 
        			std::cout<<count<<": "<<line<<std::endl;
    			}
    			firstline = false;
    			std::cout<<"共计 "<<count<<" 项日程"<<std::endl;
    			inFile.close();
				break;
			}
			
			case 5:{//删除30天以前的数据
				//初始化 
				for(int i = 0;i < 500;i++){
					et[i].info = false;
				}
				//读取文件
				std::ifstream inFile(fn);  // 创建输入文件流对象
				if (!inFile.is_open()) {
        			std::cerr << "无法打开文件: " << fn << std::endl;
        			system("pause");
        			break;
    			}
				//逐行读入ls
    			std::string line5,ls5[501];
    			int etcount5 = 0;
    			firstline = false;
    			while (std::getline(inFile, line5)) {
    				if(firstline == false){
    					ls5[500] = line5;
    					firstline = true;
    					continue;
					}
					etcount5++;
					if(line5 == ""){
    					et[etcount5].info = true;
    					std::cout<<"[拟删除] "<<etcount5<<": "<<line5<<std::endl;
    					continue;
					}
					ls5[etcount5] = line5;
					lsy = strtoint(line5.substr(0, 4));
					lsm = strtoint(line5.substr(4, 2));
					lsd = strtoint(line5.substr(6, 2));
        			if (lsy != 0 && lsm != 0 && lsd != 0){
        				int ls5 = daojishi(lsd,lsm,lsy,time.wDay,time.wMonth,time.wYear);
        				if(ls5 > 30){
        					et[etcount5].info = true;
        					std::cout<<"[拟删除] "<<etcount5<<": "<<line5<<" 相隔 "<<ls5<<" 天"<<std::endl;
						}
					}
    			}
    			firstline = false;
    			inFile.close();
    			
    			std::cout<<"输入[1]确认删除 否则不执行删除命令"<<std::endl<<"input>> ";
    			std::cin>>order;
    			if(order != 1){
    				std::cout<<"正在退出删除操作"<<std::endl;
    				for(int i = 0;i < 500;i++){
						et[i].info = false;
					}
    				std::cout<<"已经退出删除操作"<<std::endl;
    				break;
				}
    			std::ofstream outFile(fn); // 创建输出文件流对象
    			if (!outFile.is_open()) {
        			std::cerr << "无法打开文件进行写入: " << fn << std::endl;
   				}
   				outFile <<ls5[500]<<"\n";//写入理发数据(特判 
				for(int i = 1;i <= etcount5;i++){
   					if(et[i].info != true){
   							outFile <<ls5[i]<<"\n";
					}
				}
				outFile.close(); // 关闭文件
				std::cout<<"已删除"<<std::endl;
				break;
		   }
			
			case 6:{//刷新 + 重新输出
    			//初始化 
				for(int i = 0;i < 500;i++){
					et[i].year = -2;
					et[i].month = -2;
					et[i].day = -2;
					et[i].num = -2;
				}
				
				//读取文件
				std::ifstream inFile(fn);  // 创建输入文件流对象
				if (!inFile.is_open()) {
        			std::cerr << "无法打开文件: " << fn << std::endl;
        			system("pause");
        			return 1;
    			}
				//逐行读入结构体 
    			std::string line;
    			int etcount = 0;
    			firstline = false;
    			while (std::getline(inFile, line)){
    				if(firstline == false){//特判第一行  直接把理发信息输出掉 
    					firstline = true;
    					std::cout<<std::endl<<"=>下次理发时间为: "<<strtoint(line.substr(0, 4))<<" 年 "<<strtoint(line.substr(4, 2))<<" 月 "<<strtoint(line.substr(6, 2))<<" 日 星期 "<<
							week(strtoint(line.substr(0, 4)),strtoint(line.substr(4, 2)),strtoint(line.substr(6, 2)))<<" , 还有 ["<<
							daojishi(time.wDay,time.wMonth,time.wYear,strtoint(line.substr(6, 2)),strtoint(line.substr(4, 2)),strtoint(line.substr(0, 4)))<<"] 天"<<std::endl<<std::endl;
						continue;
					}
					if(line == ""){
						continue;
					}
    				//       0123456789
    				//格式为:yyyymmddXXXXXXXXXXXXXXXX 
        			etcount++;
        			et[etcount].year = strtoint(line.substr(0, 4));//substr截取子串 
        			et[etcount].month = strtoint(line.substr(4, 2));
        			et[etcount].day = strtoint(line.substr(6, 2));
        			et[etcount].event = line.substr(8);
        			et[etcount].num = etcount;
    			}
    			firstline = false;
    			inFile.close();
				//处理日程信息 
    			int thatmonth = 0,thatday = 0;
				int th[500],td[500];
				for(int i = 0;i < etcount + 1;i++){
					if(et[i].year > -2){
						if(et[i].year == 0 || et[i].year == time.wYear){
							if(et[i].month == 0 || et[i].month == time.wMonth){
								th[thatmonth] = i;
								thatmonth++;
								if(et[i].day == 0 ||et[i].day == time.wDay){
									td[thatday] = i;
									thatday++;
								}
							}
						}
					}
				}

				//输出日程信息 
				std::cout<<"文件内共计 "<<etcount + 1<<" 件日程"<<std::endl;
				std::cout<<std::endl<<"本月事件共有 "<<thatmonth<<" 件:"<<std::endl;
				for(int i = 0;i < thatmonth;i++){
					std::cout<<"    "<<i + 1<<" : "<<et[th[i]].month<<"/"<<et[th[i]].day<<" "<<et[th[i]].event<<std::endl; 
				}
				std::cout<<std::endl<<"本日事件共有 "<<thatday<<" 件:"<<std::endl;
				for(int i = 0;i < thatday;i++){
					std::cout<<"    "<<i + 1<<" : "<<et[td[i]].event<<std::endl; 
				}
				break;
			}
			
			case 7:{ //查询若间隔的日期
				std::cout<<"知道起日期和间隔计算讫日期[1]       知道起讫日期计算间隔[2]"<<std::endl<<"input>> ";
				std::cin>>order;
				if(order == 1){
					std::cout<<"间隔天数?(列如:未来两天为2,过去两天为-2)"<<std::endl<<"input>> ";
					int xn7;
					std::cin>>xn7;
					std::cout<<"起始日期?"<<std::endl;
					lsz = input_ymd(); 
					calcday(strtoint(lsz.substr(0, 4)),strtoint(lsz.substr(4, 2)),strtoint(lsz.substr(6, 2)),xn7);
					std::cout<<"距离"<<xn7<<"天以后为  "<<lsy<<" 年 "<<lsm<<" 月 "<<lsd<<" 日  "<<"星期 "<<week(lsy,lsm,lsd)<<std::endl;
				}else if(order == 2){
					std::cout<<"起始日期?"<<std::endl;
					lsz = input_ymd();
					lsy = strtoint(lsz.substr(0, 4));
					lsm = strtoint(lsz.substr(4, 2));
					lsd = strtoint(lsz.substr(6, 2));
					std::cout<<"讫止日期?"<<std::endl;
					lsz = input_ymd();
					std::cout<<"相差 "<<daojishi(lsd,lsm,lsy,strtoint(lsz.substr(6, 2)),strtoint(lsz.substr(4, 2)),strtoint(lsz.substr(0, 4)))<<" 天"<<std::endl;
				}else{
					std::cout<<"非法输入 重来吧宝"<<std::endl;
				} 
				break;
			}
			
			case 8:{//更新理发信息
				std::cout<<"自动输入当前时间[1]       手动输入[2]"<<std::endl<<"input>> ";
				std::cin>>order;
				int lsy8,lsm8,lsd8; 
				if(order == 1){
					lsy8 = time.wYear;
					lsm8 = time.wMonth;
					lsd8 = time.wDay;
				}else if(order == 2){
					std::cout<<"指定日期?"<<std::endl;
					lsz = input_ymd();
					lsy8 = strtoint(lsz.substr(0, 4));
					lsm8 = strtoint(lsz.substr(4, 2));
					lsd8 = strtoint(lsz.substr(6, 2));
				}else{
					std::cout<<"非法输入 重来吧宝"<<std::endl;
					break; 
				}
				calcday(lsy8,lsm8,lsd8,21);
				//int->str并且补足位
				std::stringstream ss8;
				std::string ls8 = "";
				//流入数据 
				ss8<<lsy<<" "<<lsm<<" "<<lsd;
				
				ss8>>lsz;//取出并转换为str 
				for(int i = lsz.size();i < 4;i++) ls8 +='0';//补0 
				ls8 += lsz;
				
				ss8>>lsz;//取出并转换为str 
				for(int i = lsz.size();i < 2;i++) ls8 +='0';//补0 
				ls8 += lsz;
				
				ss8>>lsz;//取出并转换为str 
				for(int i = lsz.size();i < 2;i++) ls8 +='0';//补0 
				ls8 += lsz;
				
				ss8.str("");  // 清空数据
				ss8.clear(); //重置状态 
				//ss8 string流处理完成
				//下面开始写入文件
				
				//读取文件
				std::ifstream inFile(fn);  // 创建输入文件流对象
				if (!inFile.is_open()) {
        			std::cerr << "无法打开文件: " << fn << std::endl;
        			system("pause");
					break; 
    			}
			    //逐行读入ls88
    			std::string line8,ls88[501];
    			int etcount8 = 0;
    			firstline = false;
    			while (std::getline(inFile, line8)) {
    				if(firstline == false){
    					firstline = true;
    					continue;
					}
					if(line8 == ""){
						continue;
					}
        			etcount8++;
					ls88[etcount8] = line8; 
    			}
    			firstline = false;
    			inFile.close();
    			std::ofstream outFile(fn); // 创建输出文件流对象
    			if (!outFile.is_open()) {
        			std::cerr << "无法打开文件进行写入: " << fn << std::endl;
        			break;
   				}
   				outFile <<ls8<<"\n";
				for(int i = 1;i <= etcount8;i++){
   					outFile <<ls88[i]<<"\n";
				}
				outFile.close(); // 关闭文件
				 
				break;
			}
			
			case 9 :{//关于本程序 
				printf("             *#^##^*                                                                                 \n"); 
				printf("            *########*                                                                               \n"); 
				printf("           #### ! <)##                                                                               \n"); 
				printf("          #####    #####                                                                        	 \n"); 
				printf("         ###   |##r  ####                      _##______      ___##___        #______      _______   \n"); 
				printf("        *### /:.#:#.   ###                    |__  /___ |     |________|     |_______|    / _____  | \n"); 
				printf("       *#&#  d.:(@}{@)  ####                    / / || ||    ___||__||___      ________   | /    / / \n"); 
				printf("      *####  01 #######  ####                  / /  || ||   |____________|   |________|   |/    / /  \n"); 
				printf("     *####   ##I #######  #####               / /|| || ||     |________|       ________        / /   \n"); 
				printf("    *#%###   ##;  :#####:   ##$##            /_/||  || ||     |______| |      |______  |      / /    \n"); 
				printf("   *#####   /##   ($=[]=$)   ######             ||  || ||     |________|             | |     / /     \n"); 
				printf("  /*##$#*   ###  /%$&###&$%    ###$##           H A T S U N E    M I K U with 项宸恺 |_|    /_/      \n"); 
				printf("  #####/   *### /%$########$%   ######                                                               \n"); 
				printf(" *#####   /####/%$##########$%  *###$##                                                              \n"); 
				printf("*######   ##### %#############%  *#####                                                              \n"); 
				printf("*##&### .#######|wom#wom#wom#wo#* *#&###                                                             \n"); 
				printf("*#####* .##=#=#/_I_### ###_I_##/   *#####                                                            \n"); 
				printf("*#####/   (%:  * - -*    *- -*      #####                                                            \n"); 
				printf("*#####*        :  |)      |  )     *#%$#                                                             \n"); 
				printf("(%#%#*         -H#/        XH0     /:.#                                                              \n"); 
				printf(" （$.%#*        =##        ###(     #*#                                                              \n"); 
				printf("  (#:.:#      ###         l#d       /                                                                \n"); 
				printf("    (*/      (#}           {#)                                                                       \n"); 
				printf("            /#/            :##                                                                       \n"); 
				printf("           /#/              ##:                                                                      \n"); 
				printf("           ##               ##                                                                       \n"); 
				printf("          /-=              /%=                                                                       \n"); 
				printf("         (=$#              ##$:                                                                      \n"); 
				printf("         |3__)            9__/                                                                     \n\n");
				printf("程序名 事件提醒\n");
				printf("原作者保留所有权利\n");
				printf("作者 项宸恺 联系方式 xiangchenkai@outlook.com \n\n");
				printf("说明 \n");
				printf("1,文件格式:\n");
				printf("	文件名 daily\n");
				printf("	(1)yyyymmdd(下次理发日期)\n");
				printf("	(2)yyyymmdd事件\n");
				printf("	(3)yyyymmdd事件\n");
				printf("	...... \n");
				printf("2,操作\n");
				printf("	退出[1]\n");
				printf("	增加日程[2]\n");
				printf("	删除日程[3]\n");
				printf("	查询文件内所有日程[4]\n");
				printf("	删除30天以前的数据[5]\n");
				printf("	刷新并且重新输出[6]\n");
				printf("	计算间隔日期[7]\n");
				printf("		知道起日期和间隔计算讫日期[1]\n");
				printf("		知道起讫日期计算间隔[2]\n");
				printf("	更新理发信息[8]\n");
				printf("		自动输入当前时间[1]\n");
				printf("		手动输入[2]\n");
				printf("	关于本程序[9]\n");
				printf("	清屏[0]\n");
				printf("3,最多500项日程\n");
				
				break;
			}
			
    		default:{
    			std::cout<<"输入非法 检查后重新输入"<<std::endl;	
				break;
			}
        	
		}	
	} 
    return 0;
}
