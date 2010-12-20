package com.gapso.web.trieda.client.util.view;

import java.util.HashMap;
import java.util.Map;

import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.ui.KeyboardListener;

public class TextFieldMask extends TextField<String> {

	public class Settings {
		private String placeHolder;
		
		public Settings() {
		}
		
		public Settings(String placeHolder) {
			this.placeHolder = placeHolder;
		}
		
		public String getPlaceHolder() {
			return placeHolder;
		}

		public void setPlaceHolder(String placeHolder) {
			this.placeHolder = placeHolder;
		}
	}

	private final String mask;
	private int len;
	private Settings settings;
	private int partialPosition;
	private String[] buffer;
	private boolean ignore;
	private String focusText;
	private String[] tests;
	private Integer firstNonMaskPos;
	private int cursorBegin = -1;
	private int cursorEnd = -1;
	private static final Map<String, String> defs;

	static {
		defs = new HashMap<String, String>();
		defs.put("9", "[0-9]");
		defs.put("a", "[A-Za-z]");
		defs.put("*", "[A-Za-z0-9]");
	}

	private static boolean cotainDef(String key) {
		if (defs.get(key) != null) {
			return true;
		} else {
			return false;
		}
	}

	private static String getDef(String key) {
		return defs.get(key);
	}

	public TextFieldMask(String mask) {
		this.mask = mask;
	}

	private void buffer() {
		String[] aux = split(mask);
		buffer = new String[aux.length];
		for (int i = 0; i < aux.length; i++) {
			if (cotainDef(aux[i])) {
				buffer[i] = settings.getPlaceHolder();
			} else {
				buffer[i] = aux[i];
			}
		}
	}

	private int checkVal(boolean allow) {
		String test = "";

		if (getValue() != null) {
			test = getValue();
		}

		int lastMatch = -1;
		int a = 0;

		for (int i = 0, pos = 0; i < len; i++) {
			if (tests[i] != null) {
				buffer[i] = settings.getPlaceHolder();
				while (pos++ < test.length()) {
					String c = String.valueOf(test.charAt(pos - 1));
					if (c.matches(tests[i])) {
						buffer[i] = String.valueOf(c);
						lastMatch = i;
						break;
					}
				}
				if (pos > test.length()) {
					break;
				}
			} else if (i != partialPosition) {
				try {
					char d = test.charAt(pos);
					if (buffer[i].equals(String.valueOf(d))) {
						pos++;
						lastMatch = i;
					}
				} catch (Exception e) {
					continue;
				}
			}
			a = i;
		}

		if (!allow && lastMatch + 1 < partialPosition) {
			setValue("");
			clearBuffer(0, len);
		} else if (allow || lastMatch + 1 >= partialPosition) {
			writeBuffer();
			if (!allow) {
				if (getValue() != null) {
					setValue(getValue().substring(0, lastMatch + 1));
				}
			}
		}
		return a;
	}

	private void clearBuffer(int start, int end) {
		for (int i = start; i < end && i < len; i++) {
			if (tests[i] != null) {
				buffer[i] = settings.getPlaceHolder();
			}
		}
	}

	private void each() {
		for (int i = 0; i < tests.length; i++) {
			String c = tests[i];
			if (c == "?") {
				len--;
				partialPosition = i;
			} else if (cotainDef(c)) {
				tests[i] = getDef(c);
				if (firstNonMaskPos == null) {
					firstNonMaskPos = tests.length - 1;
				}
			} else {
				tests[i] = null;
			}
		}
	}

	private void maskField() {
		settings = new Settings("_");
		tests = new String[] {};
		partialPosition = mask.length();
		firstNonMaskPos = null;
		len = mask.length();
		tests = split(mask);

		each();
		buffer();

		ignore = false;
		focusText = "";

		if (getValue() != null) {
			focusText = getValue();
		}

		if (!isReadOnly()) {
			checkVal(false);
		}

	}

	@Override
	protected void onBlur(ComponentEvent be) {
		super.onBlur(be);
		checkVal(false);
	}

	@Override
	protected void onFocus(ComponentEvent be) {
		super.onFocus(be);
		focusText = "";

		if (getValue() != null) {
			focusText = getValue();
		}

		int pos = checkVal(false);
		writeBuffer();

		if (pos == mask.length()) {
			cursorBegin = 0;
			cursorEnd = pos;
			select(0, pos);
		} else {
			cursorBegin = pos;
			cursorEnd = pos;
			setCursorPos(pos);
		}
	}

	@Override
	protected void onKeyDown(FieldEvent fe) {
		super.onKeyDown(fe);
		
		int k = fe.getKeyCode();
		ignore = k < 16 || k > 16 && k < 32 || k > 32 && k < 41;

		// delete selection before proceeding
		if (cursorBegin - cursorEnd != 0 && (!ignore || k == KeyboardListener.KEY_BACKSPACE || k == KeyboardListener.KEY_DELETE)) {
			clearBuffer(cursorBegin, cursorEnd);
		}

		// backspace, delete, and escape get special treatment
		if (k == KeyboardListener.KEY_BACKSPACE || k == KeyboardListener.KEY_DELETE) {
			shiftL(getCursorPos() + (k == KeyboardListener.KEY_DELETE ? 0 : -1));
			fe.stopEvent();
		} else if (k == KeyboardListener.KEY_ESCAPE) {// escape
			setValue(focusText);
			fe.stopEvent();
		}
	}

	@Override
	protected void onKeyPress(FieldEvent fe) {
		super.onKeyPress(fe);

		int k = fe.getKeyCode();

		if (ignore) {
			// Fixes Mac FF bug on backspace
			if (k == KeyboardListener.KEY_BACKSPACE) {
				fe.stopEvent();
			}
			return;
		}

		if (fe.isControlKey() || fe.isAltKey()) {// Ignore
			fe.stopEvent();
		} else if (k >= 32 && k <= 125 || k > 186) {// typeable characters
			int p = seekNext(getCursorPos() - 1);
			if (p < len) {
				String c = String.valueOf((char) fe.getKeyCode());
				if (c.matches(tests[p])) {
					shiftR(p);
					buffer[p] = c;
					writeBuffer();
					int next = seekNext(p);

					setCursorPos(next);

					cursorBegin = next;
					cursorEnd = next;
				}
			}
		}
		fe.stopEvent();
	}

	@Override
	protected void onRender(Element target, int index) {
		super.onRender(target, index);
		maskField();
	}

	private int seekNext(int index) {
		while (++index <= len) {
			try {
				if (tests[index] != null) {
					break;
				}
			} catch (Exception e) {
				break;
			}
		}
		return index;
	}

	private void shiftL(int index) {
		for (int i = index; i >= 0; i--) {
			if (tests[i] != null) {
				index = i;
				break;
			}
		}

		for (int i = index; i < len && i >= 0; i++) {
			if (tests[i] != null) {
				buffer[i] = settings.getPlaceHolder();
				int j = seekNext(i);
				if (j < len && buffer[j].matches(tests[i])) {
					buffer[i] = buffer[j];
				} else {
					break;
				}
			}
		}

		writeBuffer();
		setCursorPos(index);
	}

	private void shiftR(int index) {
		String c = settings.getPlaceHolder();

		for (int i = index; i < len; i++) {
			if (tests[i] != null) {
				int j = seekNext(i);
				String t = buffer[i];
				buffer[i] = c;
				if (j < len && t.matches(tests[j])) {
					c = t;
				} else {
					break;
				}
			}
		}
	}

	private String[] split(String text) {
		int length = text.length();
		String[] array = new String[length];

		for (int i = 0; i < length; i++) {
			array[i] = String.valueOf(text.charAt(i));
		}

		return array;
	}

	private void writeBuffer() {
		String valueAux = "";

		for (String element2 : buffer) {
			valueAux += element2;
		}
		setValue(valueAux);
	}

}