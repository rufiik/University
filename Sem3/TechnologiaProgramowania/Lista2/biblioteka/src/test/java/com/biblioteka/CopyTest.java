package com.biblioteka;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class CopyTest {
    private Copy copy;

    @Before
    public void setUp() {
        copy = new Copy();
    }

    @Test
    public void testIsBorrowed() {
        assertFalse(copy.isBorrowed());
    }

    @Test
    public void testSetBorrowed() {
        copy.setBorrowed(true);
        assertTrue(copy.isBorrowed());
    }
}