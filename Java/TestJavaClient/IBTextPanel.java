/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.StringTokenizer;

import javax.swing.BorderFactory;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.border.Border;

class IBTextPanel extends JPanel {
    public static final Color  textBackgroundColor = new Color(5, 5, 5);
    public static final Color  textForegroundColor = new Color(0, 245, 0);
    public static final Font   textComponentFont = new JList().getFont();
    public static final Color  textCaretColor = Color.WHITE;
    public static final String lineSeparator = System.getProperty("line.separator");

    private JTextArea m_textArea = new JTextArea();
    private JScrollPane m_scrollPane = new JScrollPane(m_textArea);
    private final static String LF = "\n" ;
    private final static String TAB = "\t" ;
    private final static String EIGHT_SPACES = "        ";
    private final static String EMPTY_STRING = "";

    IBTextPanel() {
        this(null, false);
    }

    IBTextPanel(String title, boolean editable) {
        super(new BorderLayout());
        if (title != null) {
            Border border = BorderFactory.createTitledBorder( title);
            setBorder(border);
        }
        m_textArea.setBackground(textBackgroundColor);
        m_textArea.setForeground(textForegroundColor);
        m_textArea.setFont(textComponentFont);
        m_textArea.setCaretColor(textCaretColor);
        m_textArea.setEditable(editable);
        add(m_scrollPane);
    }

    public void clear() {
        m_textArea.setText(EMPTY_STRING);
    }

    public void setText(String text) {
        m_textArea.setText(text);
        if (m_textArea.isEditable()) {
            moveCursorToBeginning();
        } else {
            moveCursorToEnd();
        }
    }

    public void setTextDetabbed(String text) {
        m_textArea.setText(detabbed(text));
    }

    public String getText() {
        return m_textArea.getText();
    }

    public void add(String line) {
        m_textArea.append(line + lineSeparator);
        moveCursorToEnd();
    }

    public void moveCursorToEnd() {
        m_textArea.setCaretPosition(m_textArea.getText().length());
    }

    public void moveCursorToBeginning() {
        m_textArea.setCaretPosition(0);
    }

    public void add(Collection<String> lines) {
        for (Iterator<String> iter = lines.iterator(); iter.hasNext(); ) {
            add(iter.next());
        }
    }

    public void addText(String text) {
        add(tokenizedIntoArrayList(detabbed(text), LF));
    }

    public static ArrayList<String> tokenizedIntoArrayList(String source, String delimiter) {
        ArrayList<String> list = new ArrayList<String>();
        StringTokenizer st = new StringTokenizer(source, delimiter);
        while (st.hasMoreTokens()) {
            String temp = st.nextToken();
            list.add(temp);
        }
        return list;
    }

    private static String detabbed(String text) {
        return text.replaceAll(TAB, EIGHT_SPACES);
    }
}
