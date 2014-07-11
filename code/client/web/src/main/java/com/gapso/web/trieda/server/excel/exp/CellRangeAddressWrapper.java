package com.gapso.web.trieda.server.excel.exp;

import org.apache.poi.ss.util.CellRangeAddress;

public class CellRangeAddressWrapper implements Comparable<CellRangeAddressWrapper> {  
	   
    public CellRangeAddress range;  
      
    public CellRangeAddressWrapper(CellRangeAddress theRange) {  
          this.range = theRange;  
    }  
      
    public int compareTo(CellRangeAddressWrapper o) {  
    	if (range.getFirstColumn() < o.range.getFirstColumn()  
    			&& range.getFirstRow() < o.range.getFirstRow()) {  
    		return -1;  
    	} else if (range.getFirstColumn() == o.range.getFirstColumn()  
    			&& range.getFirstRow() == o.range.getFirstRow()) {  
    		return 0;  
    	} else {  
    		return 1;  
    	}  
    }  
}  
