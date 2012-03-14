package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.IconButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.ProgressReportServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AcompanhamentoPanelPresenter implements Presenter{
	
	public interface Display {
		ContentPanel getMessagesPanel();
		SimpleModal getSimpleModal();
		Button getOkButton();
		ToolButton getCloseButton();
		void updateMessagePanel(String msg);
	}

	private ProgressReportServiceAsync progressSource;
	private Display display;
	private Timer refreshTimer;
	private String progressKey;
	private int tentativas;

	public AcompanhamentoPanelPresenter(String progressKey, Display display){
		this.progressKey = progressKey;
		this.display = display;
		tentativas = 0;
		progressSource = Services.progressReport();
		
		initUI();
		getProgressReport();
	}
	
	public void initUI(){
		display.getOkButton().disable();
		setUIListeners();
	}
	
	private void setUIListeners(){
		display.getOkButton().addSelectionListener(
			new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					display.getSimpleModal().hide();
				}
			}
		);

		if(display.getCloseButton() != null){
			display.getCloseButton().addSelectionListener(
				new SelectionListener< IconButtonEvent >(){
				@Override
				public void componentSelected(IconButtonEvent ce){
				}
			});
		}
	}
	
	public void getProgressReport(){
		progressSource.isReadyToRead(progressKey, new AsyncCallback<Boolean>(){
			@Override
			public void onSuccess(Boolean result){
				if(result){
					go(null);
					setListeners();
				}
				else{
					if(tentativas++ == 10) return;
					Timer getProgressReportTimer = new Timer(){
						@Override
						public void run(){
							getProgressReport();
						}
					};
					getProgressReportTimer.schedule(1 * 1000);
				}
			}
			
			@Override
			public void onFailure(Throwable t){
				display.updateMessagePanel(t.getMessage());
			}
		});
	}

	private void setListeners(){
		refreshTimer = new Timer(){
			@Override
			public void run(){
				refreshMessageList();
			}
		};
		refreshTimer.schedule(1 * 1000);
	}
	
	public void refreshMessageList(){
		progressSource.getProgressMessages(progressKey, new AsyncCallback<List<String>>(){
			@Override
			public void onSuccess(List<String> messages){
				for(String msg: messages) display.updateMessagePanel(msg);
				
				progressSource.isFinished(progressKey, new AsyncCallback<Boolean>(){
					@Override
					public void onSuccess(Boolean isFinished){
						if(!isFinished) refreshTimer.schedule(1 * 1000);
						else {
							display.getOkButton().enable();
							verifyClosing();
						}
					}
					
					@Override
					public void onFailure(Throwable t){
						display.updateMessagePanel(t.getMessage());
					}
				});
			}
			
			@Override
			public void onFailure(Throwable t){
				display.updateMessagePanel(t.getMessage());
			}
		});
	}
	
	public void verifyClosing(){
		refreshTimer = new Timer(){
			@Override
			public void run(){
				progressSource.isClosed(progressKey, new AsyncCallback<Boolean>(){
					
					@Override
					public void onSuccess(Boolean isClosed){
						if(!isClosed) refreshTimer.schedule(1 * 60 * 1000);
					}
					
					@Override
					public void onFailure(Throwable t){
						display.updateMessagePanel(t.getMessage());
					}
				});
			}
		};
		refreshTimer.schedule(1 * 1000);
	}

	@Override
	public void go(Widget widget){
		display.getSimpleModal().show();
	}
}
