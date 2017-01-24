#include "htmlreporter.h"
#include "common.h"
#include <chrono>

HtmlReporter::HtmlReporter(string filename, vector<Fusion>& fusionList, vector<Match*> *fusionMatches){
    mFusionList = fusionList;
    mFusionMatches = fusionMatches;
    mFilename = filename;
    mFile.open(mFilename.c_str(), ifstream::out);
}

HtmlReporter::~HtmlReporter(){
    mFile.close();
}

void HtmlReporter::run() {
    printHeader();
    printHelper();
    printFusions();
    printFooter();
}

void HtmlReporter::printHelper() {
    mFile << "<div id='helper'><p>Helpful tips:</p><ul>";
    mFile << "<li> Base color indicates quality: <font color='#78C6B9'>extremely high (Q40+)</font>, <font color='#33BBE2'>high (Q30+)</font>, <font color='#666666'>moderate (Q20+)</font>, <font color='#E99E5B'>low (Q15+)</font>, <font color='#FF0000'>extremely low (0~Q14)</font> </li>";
    mFile << "<li> Move mouse over the base, it will show the quality value</li>";
    mFile << "<li> Click on any row, the original read/pair will be displayed</li>";
    mFile << "<li> In first column, <i>d</i> means the edit distance of match, and --> means forward, <-- means reverse </li>";
    mFile << "<li> For pair-end sequencing, MutScan tries to merge each pair, and the overlapped bases will be assigned higher qualities </li>";
    mFile << "</ul></div>";
}

void HtmlReporter::printFusions() {
    // calculate the found fusion
    /*
    int found = 0;
    for(int i=0;i<mFusionList.size();i++){
        vector<Match*> matches = mFusionMatches[i];
        if(matches.size()>0){
            found++;
        }
    }
    // print menu
    mFile<<"<div id='menu'><p>Found "<< found << " fusion";
    if(found>1)
        mFile<<"s";
    mFile<<":</p><ul>";
    int id = 0;
    for(int i=0;i<mFusionList.size();i++){
        vector<Match*> matches = mFusionMatches[i];
        if(matches.size()>0){
            id++;
            mFile<<"<li class='menu_item'><a href='#"<<mFusionList[i].mName<<"'> " << id << ", " << mFusionList[i].mName;
            mFile<< " (" << matches.size() << " reads support, " << Match::countUnique(matches) << " unique)" << "</a></li>";
        }
    }
    mFile<<"</ul></div>";
    id=0;
    for(int i=0;i<mFusionList.size();i++){
        vector<Match*> matches = mFusionMatches[i];
        if(matches.size()>0){
            id++;
            printFusion(id, mFusionList[i], matches);
        }
    }*/
}

void HtmlReporter::printFusion(int id, Fusion& fusion, vector<Match*>& matches){
    /*
    mFile << "<div class='fusion_block'>";
    mFile << "<div class='fusion_head'><a name='" << fusion.mName << "'>";
    mFile << id << ", " << fusion.mName<< " (" << matches.size() << " reads support, " << Match::countUnique(matches) << " unique)" ;
    mFile << "</a></div>";
    mFile << "<table>";
    mFile << "<tr class='header'>";
    mFile << "<td class='match_brief'>" << "ID_Distance_Strand" << "</td>";
    mFile << "<td>" << "" << "</td>";
    mFile << "<td>" << fusion.mLeft << "</td>";
    mFile << "<td>" << fusion.getCenterHtml() << "</td>";
    mFile << "<td>" << fusion.mRight << "</td>";
    mFile << "<td>" << "" << "</td>";
    mFile << "</tr>";
    for(int m=0; m<matches.size(); m++){
        long rowid = id*100000 + m;
        mFile << "<tr onclick='toggle(" << rowid << ");'>";
        mFile << "<td>";
        // for display alignment
        if(m+1<10)
            mFile<<"0";
        if(m+1<100)
            mFile<<"0";
        if(m+1<1000)
            mFile<<"0";
        mFile << m+1 << ", ";
        matches[m]->printHtmlTD(mFile, fusion.mLeft.length(), fusion.mCenter.length(), fusion.mRight.length());
        mFile << "</tr>";
        // print a hidden row containing the full read
        mFile << "<tr id='" << rowid << "' style='display:none;'>";
        mFile << "<td colspan='6'><xmp>";
        matches[m]->printReadsToFile(mFile);
        mFile << "</xmp></td>";
        mFile << "</tr>";
    }
    mFile << "</table></div>";
    */
}

void HtmlReporter::printHeader(){
    mFile << "<html><head><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\" />";
    mFile << "<title>MutScan report</title>";
    printJS();
    printCSS();
    mFile << "</head>";
    mFile << "<body><div id='container'>";
}

void HtmlReporter::printCSS(){
    mFile << "<style type=\"text/css\">";
    mFile << "td {border:1px solid #dddddd;padding-left:2px;padding-right:2px;font-size:10px;}";
    mFile << "table {border:1px solid #999999;padding:2x;border-collapse:collapse;}";
    mFile << "img {padding:30px;}";
    mFile << ".alignleft {text-align:left;}";
    mFile << ".alignright {text-align:right;}";
    mFile << ".header {color:#ffffff;padding:1px;height:20px;background:#000000;}";
    mFile << ".figuretitle {color:#996657;font-size:20px;padding:50px;}";
    mFile << "#container {text-align:center;padding:1px;font-family:Arial;}";
    mFile << "#menu {padding-top:10px;padding-bottom:10px;text-align:left;}";
    mFile << ".menu_item {text-align:left;padding-top:5px;font-size:18px;}";
    mFile << ".highlight {text-align:left;padding-top:30px;padding-bottom:30px;font-size:20px;line-height:35px;}";
    mFile << ".fusion_head {text-align:left;color:#0092FF;font-family:Arial;padding-top:20px;padding-bottom:5px;}";
    mFile << ".fusion_block {}";
    mFile << ".match_brief {font-size:8px}";
    mFile << ".fusion_point {color:#FFCCAA}";
    mFile << "#helper {text-align:left;border:1px dotted #fafafa;color:#777777;}";
    mFile << "#footer {text-align:left;padding-left:10px;padding-top:20px;color:#777777;font-size:10px;}";
    mFile << "</style>";
}

void HtmlReporter::printJS(){
    mFile << "<script type=\"text/javascript\">" << endl;
    mFile << "function toggle(targetid){ \n\
                if (document.getElementById){ \n\
                    target=document.getElementById(targetid); \n\
                        if (target.style.display=='table-row'){ \n\
                            target.style.display='none'; \n\
                        } else { \n\
                            target.style.display='table-row'; \n\
                        } \n\
                } \n\
            }";
    mFile << "function toggle_target_list(targetid){ \n\
                if (document.getElementById){ \n\
                    target=document.getElementById(targetid); \n\
                        if (target.style.display=='block'){ \n\
                            target.style.display='none'; \n\
                            document.getElementById('target_view_btn').value='view';\n\
                        } else { \n\
                            document.getElementById('target_view_btn').value='hide';\n\
                            target.style.display='block'; \n\
                        } \n\
                } \n\
            }";
    mFile << "</script>";
}

const std::string getCurrentSystemTime()
{
  auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm* ptm = localtime(&tt);
  char date[60] = {0};
  sprintf(date, "%d-%02d-%02d      %02d:%02d:%02d",
    (int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday,
    (int)ptm->tm_hour,(int)ptm->tm_min,(int)ptm->tm_sec);
  return std::string(date);
}

extern string command;

void HtmlReporter::printFooter(){
    mFile << "<div id='footer'> ";
    mFile << "<p>"<<command<<"</p>";
    printScanTargets();
    mFile << "MutScan " << FUSIONSCAN_VER << ", at " << getCurrentSystemTime() << " </div>";
    mFile << "</div></body></html>";
}

void HtmlReporter::printScanTargets(){/*
    mFile << "<div id='targets'> ";
    mFile << "<p> scanned " << mFusionList.size() << " fusion spots...<input type='button' id='target_view_btn', onclick=toggle_target_list('target_list'); value='show'></input></p>";
    mFile << "<ul id='target_list' style='display:none'>";
    int id=0;
    for(int i=0;i<mFusionList.size();i++){
        id++;
        mFile<<"<li> " << id << ", " << mFusionList[i].mName << "</li>";
    }
    mFile << "</ul></div>";
    */
}