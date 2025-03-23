package com.biblioteka;
import java.util.Scanner;

import com.biblioteka.Library.InvalidNumberOfCopiesException;
/**
 * Klasa LibraryApp jest główną klasą aplikacji bibliotecznej.
 * Umożliwia użytkownikowi dodawanie książek, egzemplarzy, czytelników,
 * wypożyczanie książek oraz wyświetlanie listy książek i czytelników.
 *  Zastosowane wzorce GRASP:
 * - Controller: Klasa LibraryApp działa jako kontroler, który obsługuje zdarzenia systemowe (wybory użytkownika).
 * - Information Expert: Klasa Library jest odpowiedzialna za zarządzanie książkami i czytelnikami, ponieważ posiada odpowiednie informacje.
 * - Low Coupling: Klasa LibraryApp ma niskie sprzężenie z innymi klasami, ponieważ używa interfejsów publicznych klasy Library.
 * - High Cohesion: Klasa LibraryApp jest odpowiedzialna tylko za interakcję z użytkownikiem, co zapewnia wysoką spójność.
 * - Pure Fabrication: Interfejs Database i jego implementacje są przykładami Pure Fabrication, ponieważ są stworzone w celu zarządzania operacjami na danych, a nie reprezentują rzeczywistych obiektów w domenie problemu.
 * - Dependency Inversion: Klasa Library zależy od interfejsu Database (abstrakcja), a nie od konkretnej implementacji bazy danych, co pozwala na łatwą zamianę implementacji bazy danych bez modyfikowania kodu klasy Library.
 */
public final class LibraryApp {
    /**ADD_BOOK - dodanie książki. */
    private static final int ADD_BOOK = 1;
    /**ADD_COPY - dodanie egzemplarza książki. */
    private static final int ADD_COPY = 2;
    /**ADD_READER - dodanie czytelnika. */
    private static final int ADD_READER = 3;
    /**BORROW_BOOK - wypożyczenie książki. */
    private static final int BORROW_BOOK = 4;
    /**DISPLAY_BOOKS - wyświetlenie książek. */
    private static final int DISPLAY_BOOKS = 5;
    /**DISPLAY_READERS - wyświetlenie czytelników. */
    private static final int DISPLAY_READERS = 6;
    /**RETURN_BOOK - oddanie książki. */
    private static final int RETURN_BOOK = 7;
    /**EXIT - wyjście z programu. */
    private static final int EXIT = 8;
    /**
     * Prywatny konstruktor, aby zapobiec instancjonowaniu klasy.
     */
    private LibraryApp() {
        // Prywatny konstruktor
    }
      /**
     * Główna metoda aplikacji bibliotecznej.
     * Wyświetla menu i obsługuje wybory użytkownika.
     * @param args argumenty wiersza poleceń (nieużywane)
     */
    public static void main(final String[] args) {
        Database database = new InMemoryDatabase();
        Library library = new Library(database);
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("Wybierz opcję:");
            System.out.println("1. Dodaj książkę");
            System.out.println("2. Dodaj egzemplarz książki");
            System.out.println("3. Dodaj czytelnika");
            System.out.println("4. Wypożycz książkę");
            System.out.println("5. Wyświetl książki");
            System.out.println("6. Wyświetl czytelników");
            System.out.println("7. Oddaj książkę");
            System.out.println("8. Wyjdź");

            int choice = scanner.nextInt();
            scanner.nextLine();  // Pobierz nową linię po wczytaniu liczby
            try{
            switch (choice) {
                case ADD_BOOK:
                    System.out.print("Podaj tytuł książki: ");
                    String title = scanner.nextLine();
                    System.out.print("Podaj liczbę egzemplarzy: ");
                    int numberOfCopies = scanner.nextInt();
                    scanner.nextLine();
                    library.addBook(title, numberOfCopies);
                    break;
                case ADD_COPY:
                    System.out.print("Podaj tytuł książki: ");
                    title = scanner.nextLine();
                    System.out.print("Podaj liczbę egzemplarzy: ");
                    numberOfCopies = scanner.nextInt();
                    scanner.nextLine();
                    library.addCopy(title, numberOfCopies, scanner);
                    break;
                case ADD_READER:
                    System.out.print("Podaj imię czytelnika: ");
                    String name = scanner.nextLine();
                    library.addReader(name);
                    break;
                case BORROW_BOOK:
                    System.out.print("Podaj tytuł książki: ");
                    title = scanner.nextLine();
                    System.out.print("Podaj imię czytelnika: ");
                    String readerName = scanner.nextLine();
                    library.borrowBook(title, readerName);
                    break;
                case DISPLAY_BOOKS:
                    library.displayBooks();
                    break;
                case DISPLAY_READERS:
                    library.displayReaders();
                    break;
                case RETURN_BOOK:
                    System.out.print("Podaj tytuł książki: ");
                    title = scanner.nextLine();
                    System.out.print("Podaj imię czytelnika: ");
                    String name1 = scanner.nextLine();
                    library.returnBook(title, name1);
                    break;
                case EXIT:
                    System.out.println("Do widzenia!");
                    scanner.close();
                    return;
                default:
                    System.out.println("Nieprawidłowy wybór.");
            }
        }catch(InvalidNumberOfCopiesException e){
            System.out.println("Błąd: " + e.getMessage());
        }
    }
    }
}
