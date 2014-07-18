import java.io.File;
import java.io.IOException;

import com.gapso.web.trieda.server.util.progressReport.ProgressReportFileWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclaration;

import java.util.ArrayList;
import java.util.List;

public aspect ProgressDeclarationAspect{
	
	private ProgressReportWriter ProgressDeclaration.progressReport;
	
	public void ProgressDeclaration.setProgressReport(List<String> fbl){
		progressReport = new ProgressReportListWriter(fbl);
	}
	
	public void ProgressDeclaration.setProgressReport(File f) throws IOException{
		progressReport = new ProgressReportFileWriter(f);
	}
	
	public ProgressReportWriter ProgressDeclaration.getProgressReport(){
		if (progressReport == null)
			progressReport = new ProgressReportListWriter(new ArrayList<String>());
		return progressReport;
	}
	
	declare parents: @ProgressDeclarationAnnotation * implements ProgressDeclaration;
	
}