/*
	Copyright 2009-2012, Sumeet Chhetri 
  
    Licensed under the Apache License, Version 2.0 (the "License"); 
    you may not use this file except in compliance with the License. 
    You may obtain a copy of the License at 
  
        http://www.apache.org/licenses/LICENSE-2.0 
  
    Unless required by applicable law or agreed to in writing, software 
    distributed under the License is distributed on an "AS IS" BASIS, 
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
    See the License for the specific language governing permissions and 
    limitations under the License.  
*/
/*
 * ComponentGen.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: sumeet
 */

#include "ComponentGen.h"

ComponentGen::ComponentGen() {
	logger = LoggerFactory::getLogger("ComponentGen");
}

ComponentGen::~ComponentGen() {
	// TODO Auto-generated destructor stub
}

std::string convertfrom(std::string val)
{
	StringUtil::toUpper(val);
	if(val=="TRUE")
		return "true";
	else if(val=="FALSE")
		return "false";
	else
	{
		throw "Conversion exception";
	}
}


std::string ComponentGen::generateComponentCU(const std::string& fileName, std::string &cudata, std::string &cuheader, std::string &curemote, std::string &curemoteheaders, const std::string& appName)
{
	std::string file;
	/*int s,en;
	s = fileName.find_last_of("/")+1;
	dir = fileName.substr(0,s-1);
	en = fileName.find_last_of(".");*/
	std::string initheaders,cuinithdrs,name;
	/*file = fileName.substr(s,en-s);
	file = ("Component_"+file);
	std::string initheaders = "#include \"Component.h\"\n";
	std::string cuheader = "class "+file + ": public Component \n{\n";
	std::string cudata = "#include \""+file+".h\"\n\n";
	std::string constr;
	std::string servs,servsh;
	cuheader += "public:\n";
	cuheader += "~"+file+"();\n";
	cuheader += file+"();\n";
	cudata += file+"::~"+file+"()\n{}\n";
	cudata += file+"::"+file+"()\n{\n";*/
	PropFileReader pread;
	propMap srprps = pread.getProperties(fileName);
	if(srprps.size()>2)
	{
		try
	    {
			name = srprps["@CMP_NAME"];
			if(name=="")
				throw "Component Name must be specified";
			file = (StringUtil::capitalizedCopy(appName)+"_Component_"+name);
			std::string cudef(file);
			StringUtil::toUpper(cudef);
			initheaders = "#ifndef "+cudef+"_H\n#define "+cudef+"_H\n";
			cuinithdrs = "#ifndef "+cudef+"_REMOTE_H\n#define "+cudef+"_REMOTE_H\n";
			initheaders += "#include \"Component.h\"\n";
			initheaders += "#include \"AppContext.h\"\n";
			cuheader = "class "+file + ": public Component \n{\n";
			curemote =  "#include \""+file+"_Remote.h\"\n";
			curemoteheaders = "#include \"RemoteComponent.h\"\n";
			curemoteheaders += "class "+file + "_Remote : public RemoteComponent \n{\n";

			curemoteheaders += "public:\n";
			curemoteheaders += "~"+file+"_Remote();\n";
			curemoteheaders += file+"_Remote();\n";

			cudata = "#include \""+file+".h\"\n\n";

			std::string constr;
			std::string servs,servsh,rsrvs;
			cuheader += "public:\n";
			cuheader += "~"+file+"();\n";
			cuheader += file+"();\n";
			cudata += file+"::~"+file+"()\n{}\n";
			cudata += file+"::"+file+"()\n{\n";
			cudata += "AppContext::lookup(\""+name+"\");\n;";

			curemote += file+"_Remote::~"+file+"_Remote()\n{}\n";
			curemote += file+"_Remote::"+file+"_Remote()\n{\n";
			curemote += ("this->setBeanName(\""+name+"\");\n");

			cudata+= ("this->setName(\""+name+"\");\n");

			std::string desc = srprps["@CMP_DESC"];
			cudata+= ("this->setDesc(\""+desc+"\");\n");

			cudata+= ("this->setAjaxAvail("+convertfrom(srprps["@AJAX_AVAIL"])+");\n");

			cudata+= ("this->setWebsAvail("+convertfrom(srprps["@WEBS_AVAIL"])+");\n");

			cudata+= ("this->setMethinvAvail("+convertfrom(srprps["@MINV_AVAIL"])+");\n");

			cudata+= ("this->setAutoTranx("+convertfrom(srprps["@AUTO_TRANS"])+");\n");

			cudata+= ("this->setSession("+convertfrom(srprps["@SESSION"])+");\n");

			cudata+= ("this->setThrdPerReq("+convertfrom(srprps["@THRD_PER_REQ"])+");\n");

			cudata+= ("this->setThrdPoolSize("+srprps["@THRD_POOL_NUM"]+");\n");

			cudata+= ("this->setAuthMode("+convertfrom(srprps["@AUTH_ALL"])+");\n");

			std::string alwd = srprps["@USR_GRP_ALWD"];
			Cont1 alwdls;
			StringUtil::split(alwdls, alwd, (","));
			for (unsigned int var = 0; var < alwdls.size(); var++)
			{
				cudata+= ("this->getAllwList().push_back(\""+alwdls.at(var)+"\");\n");
			}

			std::string blkd = srprps["@BLOCK_USERS"];
			Cont1 blkls;
			StringUtil::split(blkls, blkd, (","));
			for (unsigned int var = 0; var < blkls.size(); var++)
			{
				cudata+= ("this->getBlkdList().push_back(\""+blkls.at(var)+"\");\n");
			}

			std::string prots = srprps["@PROTO_ALWD"];
			Cont1 protls;
			StringUtil::split(protls, prots, (","));
			for (unsigned int var = 0; var < protls.size(); var++)
			{
				cudata+= ("this->getProtocols().push_back(\""+protls.at(var)+"\");\n");
			}

			ConnectionSettings consts;
			consts.mode = srprps["@AUTH_DETS"];
			cudata += ("ConnectionSettings conset(");
			cudata+= ("\""+srprps["@AUTH_DETS"]+"\",");
			consts.source = srprps["@AUTH_CONN_SRC"];
			cudata+= ("\""+srprps["@AUTH_CONN_SRC"]+"\",");
			consts.username = srprps["@AUTH_USR_NAME"];
			cudata+= ("\""+srprps["@AUTH_USR_NAME"]+"\",");
			consts.passwd = srprps["@AUTH_USR_PASS"];
			cudata+= ("\""+srprps["@AUTH_USR_PASS"]+"\",");
			consts.address = srprps["@AUTH_ADD"];
			cudata+= ("\""+srprps["@AUTH_ADD"]+"\");\n");
			cudata+= ("this->setAuthDets(conset);\n");

			cudata+= ("this->setDbconpoolSize("+srprps["@DB_CONN_POOL_NUM"]+");\n");

			int cnt =1;
			while(srprps["@SERVICE"+CastUtil::lexical_cast<std::string>(cnt)]!="")
			{
				std::string srvc = srprps["@SERVICE"+CastUtil::lexical_cast<std::string>(cnt++)];
				ComponentService cmps;
				if(srvc.find("@USR_GRP_ALWD(")!=std::string::npos)
				{
					int st = srvc.find("@USR_GRP_ALWD(");
					std::string temp = srvc.substr(st+14);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							Cont1 alwd;
							StringUtil::split(alwd, temtem, (","));
						}
					}
				}
				if(srvc.find("@BLOCK_USERS(")!=std::string::npos)
				{
					int st = srvc.find("@BLOCK_USERS(");
					std::string temp = srvc.substr(st+13);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							Cont1 alwd;
							StringUtil::split(alwd, temtem, (","));
						}
					}
				}
				if(srvc.find("@BLOCK_USR_GRPS(")!=std::string::npos)
				{
					int st = srvc.find("@BLOCK_USR_GRPS(");
					std::string temp = srvc.substr(st+16);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							Cont1 alwd;
							StringUtil::split(alwd, temtem, (","));
						}
					}
				}
				std::string srvret = "void";
				if(srvc.find("@SRV_RET(")!=std::string::npos)
				{
					int st = srvc.find("@SRV_RET(");
					std::string temp = srvc.substr(st+9);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							cmps.retType = temtem;
							srvret = temtem;
						}
					}
				}
				if(srvc.find("@USERS_ALWD(")!=std::string::npos)
				{
					int st = srvc.find("@USERS_ALWD(");
					std::string temp = srvc.substr(st+12);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							Cont1 alwd;
							StringUtil::split(alwd, temtem, (","));
						}
					}
				}
				std::string srvcls,srvmeth;
				if(srvc.find("@SIGNATURE(")!=std::string::npos)
				{
					int st = srvc.find("@SIGNATURE(");
					std::string temp = srvc.substr(st+11);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							Cont1 alwd;
							StringUtil::split(alwd, temtem, ("."));
							if(alwd.size()==2)
							{
								srvcls = alwd.at(0);
								srvmeth = alwd.at(1);
								cmps.classn = alwd.at(0);
								cmps.methName = alwd.at(1);
							}
							else
							{
								throw "Invalid Service Signature";
							}
						}
					}
				}
				std::string srvname;
				if(srvc.find("@NAME(")!=std::string::npos)
				{
					int st = srvc.find("@NAME(");
					std::string temp = srvc.substr(st+6);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							srvname = temtem;
						}
					}
				}
				std::string srvargs,srvmcarg,srvmcargremote,srvmcargremotevec="std::vector<GenericObject> _vec;\n";
				if(srvc.find("@ARGS(")!=std::string::npos)
				{
					int st = srvc.find("@ARGS(");
					std::string temp = srvc.substr(st+6);
					size_t sp = temp.find_first_of(" ");
					size_t cl = temp.find_first_of(")");
					if((sp!=std::string::npos && sp<cl) || cl==std::string::npos)
					{
						throw "No spaces allowed between in value";
					}
					else
					{
						std::string temtem = temp.substr(0,cl);
						if(temtem.find("@BLOCK_USERS")!=std::string::npos ||
								temtem.find("@USR_GRP_ALWD")!=std::string::npos ||
								temtem.find("@BLOCK_USR_GRPS")!=std::string::npos ||
								temtem.find("@SRV_RET")!=std::string::npos ||
								temtem.find("@USERS_ALWD")!=std::string::npos ||
								temtem.find("@NAME")!=std::string::npos ||
								temtem.find("@ARGS")!=std::string::npos ||
								temtem.find("@SIGNATURE")!=std::string::npos)
						{
							throw "Invalid start of property within another property";
						}
						else
						{
							if(temtem=="void")
							{}
							else
							{
								Cont1 alwd;
								StringUtil::split(alwd, temtem, (","));
								for (unsigned int var = 0; var < alwd.size(); ++var)
								{
									if(alwd.at(var)!="" && alwd.at(var).find("*")==std::string::npos && alwd.at(var).find("&")==std::string::npos)
									{
										//srvargs += alwd.at(var) + "*_"+CastUtil::lexical_cast<std::string>(var+1)+" = ("+alwd.at(var)+"*)args.at("+CastUtil::lexical_cast<std::string>(var)+");\n";
										srvmcarg += "_"+CastUtil::lexical_cast<std::string>(var+1)+"";
										srvargs += alwd.at(var)+ " _"+CastUtil::lexical_cast<std::string>(var+1);
										srvmcargremote += alwd.at(var)+ " &_"+CastUtil::lexical_cast<std::string>(var+1);
										srvmcargremotevec += "GenericObject __"+CastUtil::lexical_cast<std::string>(var+1)+";\n";
										srvmcargremotevec += "__"+CastUtil::lexical_cast<std::string>(var+1)+" << _"+CastUtil::lexical_cast<std::string>(var+1)+";\n";
										srvmcargremotevec += "_vec.push_back(__"+CastUtil::lexical_cast<std::string>(var+1)+");\n";
										if(var!=alwd.size()-1)
										{
											srvmcarg += ",";
											srvmcargremote += ",";
											srvargs += ",";
										}
									}
									else
									{
										throw "Invalid service arguments";
									}
								}
							}
						}
					}
				}
				cuinithdrs += "#include \""+srvcls+".h\"\n";
				initheaders += "#include \""+srvcls+".h\"\n";
				servsh += srvret+" "+srvname+"("+srvargs+");\n";
				servs += srvret+" "+file+"::"+srvname+"("+srvargs+")\n{\n";
				servs += srvcls +" _serviceImpl;\n";
				if(srvret!="" && srvret!="void")
					servs += srvret + " _retvalue;\n";
				servs += "try{\n";
				if(convertfrom(srprps["@AUTO_TRANS"])=="true")
					servs += "this->startTransaction();\n";
				if(srvret!="" && srvret!="void")
					servs += "_retvalue = _serviceImpl."+srvmeth+"("+srvmcarg+");\n}\n";
				else
					servs += "_serviceImpl."+srvmeth+"("+srvmcarg+");\n}\n";
				servs += "catch(...){\n";
				if(convertfrom(srprps["@AUTO_TRANS"])=="true")
					servs += "this->rollback();\n";
				servs += "}\n";
				if(convertfrom(srprps["@AUTO_TRANS"])=="true")
					servs += "this->endTransaction();\n";
				if(srvret!="" && srvret!="void")
					servs += "return _retvalue;\n}\n\n";
				else
					servs += "}\n\n";
				curemoteheaders += srvret+" "+srvname+"("+srvmcargremote+");\n";
				rsrvs += srvret+" "+file+"_Remote::"+srvname+"("+srvmcargremote+")\n{\n";
				rsrvs += srvmcargremotevec;
				if(srvret!="" && srvret!="void")
					rsrvs += "return ";
				rsrvs += "*("+srvret+"*)this->invoke(\""+srvname+"\",_vec,\""+srvret+"\");\n";
				rsrvs += "}\n";
			}
			curemoteheaders += "};\n";
			cudata += "}\n"+servs;
			curemote += "}\n"+rsrvs;
			cuheader += servsh + "};\n";
			cuheader = initheaders + cuheader+"#endif";
			curemoteheaders = cuinithdrs + curemoteheaders+"#endif";
			/*logger << cuheader << std::flush;
			logger << "\n\n\n\n\n\n------------------------------------------------------------------\n\n\n\n\n\n" << std::flush;
			logger << cudata << std::flush;
			logger << "\n\n\n\n\n\n------------------------------------------------------------------\n\n\n\n\n\n" << std::flush;
			logger << curemoteheaders << std::flush;
			logger << "\n\n\n\n\n\n------------------------------------------------------------------\n\n\n\n\n\n" << std::flush;
			logger << curemote << std::flush;*/

	    }
		catch(const char* ex)
		{
			logger << "Exception occurred" << std::flush;
			throw;
		}

	}
	else
		throw "Error Parsing Component";
	return file;
}
