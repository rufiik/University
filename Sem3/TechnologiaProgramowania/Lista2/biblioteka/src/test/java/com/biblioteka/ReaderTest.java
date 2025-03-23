package com.biblioteka;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class ReaderTest {
    private Reader reader;

    @Before
    public void setUp() {
        reader = new Reader("Test Reader");
    }

    @Test
    public void testGetName() {
        assertEquals("Test Reader", reader.getName());
    }
}