function Logger(root, name)
{
	this.rootDirectory=root;
	this.fileName=name;
	this.logList=directory(this.rootDirectory + this.fileName + "*.log"); 		
	this.logFile=new File(this.rootDirectory + this.fileName + this.logList.length + ".log");
	this.logFile.open('w+',false);
	this.Log=function(text)
	{
		this.logFile.writeln(text);
	}
	this.Log("****" + system.datestr() +" - "+ system.timestr());
}