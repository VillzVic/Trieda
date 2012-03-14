package com.gapso.web.trieda.server.util.progressReport;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;

@Aspect
public class ProgressReportAspect {

	@Pointcut("@annotation(method) && target(com.gapso.web.trieda.server.util.progressReport.ProgressDeclaration)")
	public void annotedProgressReportMethodScan(ProgressReportMethodScan method){}
	
	@Around("annotedProgressReportMethodScan(method)")
	public Object aroundAnnotedProgressReportMethodScan(ProceedingJoinPoint pjp, ProgressReportMethodScan method) throws Throwable{
		ProgressReportWriter prw = ((ProgressDeclaration) pjp.getTarget()).getProgressReport();
		String texto = method.texto();
		boolean textoNulo = texto == null;
		
		if(prw != null){
			if(textoNulo) texto = "Executando " + pjp.getSignature().getName();
			prw.setInitNewPartial(texto);
		}
		Object obj = pjp.proceed(pjp.getArgs());
		if(prw != null){
			texto = (!textoNulo) ? "Fim de " + method.texto() : 
				"Fim da execução de " + pjp.getSignature().getName();
			prw.setPartial(texto);
		}
		
		return obj;
	}

	@Pointcut("execution(* com.gapso.web.trieda.server.excel.imp.IImportExcel.load(..)) && target(com.gapso.web.trieda.server.util.progressReport.ProgressDeclaration)")
	public void interceptOfIImportExcelLoad(){}
	
	@Around("interceptOfIImportExcelLoad()")
	public Object aroundInterceptOfIImportExcelLoad(ProceedingJoinPoint pjp) throws Throwable{
		ProgressReportWriter prw = ((ProgressDeclaration) pjp.getTarget()).getProgressReport();

		if(prw != null) prw.start();
		Object obj = pjp.proceed(pjp.getArgs());
		if(prw != null) prw.finish();
		
		return obj;
	}
	/*
	@Pointcut("call(* com.gapso.web.trieda.server.excel.imp.IImportExcel.load(..)) && withincode(* com.gapso.web.trieda.server.excel.imp.IImportExcel.load(..)) && target(teste)")
	public void interceptOfSecondIImportExcelLoad(Object teste){}

	@Around("interceptOfSecondIImportExcelLoad(teste)")
	public Object aroundInterceptOfSecondIImportExcelLoad(ProceedingJoinPoint pjp, Object teste) throws Throwable{
//		ProgressReportWriter prw = ((ProgressDeclaration) pjp.getTarget()).getProgressReport();
		IImportExcel iie = ((IImportExcel) pjp.getTarget());

		System.out.println("Importando " + iie.getSheetName());
//		if(prw != null) prw.setInitNewPartial("Processando " + iie.getSheetName());
		
		Object obj = pjp.proceed(pjp.getArgs());
//		if(prw != null) prw.setPartial("Processamento de " + iie.getSheetName() + " concluído.");
		
		return obj;
	}
	*/
	
}
