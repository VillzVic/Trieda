package com.gapso.web.trieda.main.client.command.util;

import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.gapso.web.trieda.main.client.command.ICommand;

public class CommandSelectionListener<E extends ComponentEvent> extends SelectionListener<E> {
	
	private ICommand command;
	
	static public <E extends ComponentEvent> CommandSelectionListener<E> create(ICommand command) {
		return new CommandSelectionListener<E>(command);
	}
	
	public CommandSelectionListener(ICommand command) {
		this.command = command;
	}
	
	@Override
	public void componentSelected(E ce) {
		if (command != null) {
			command.execute();
		}
	}
}