package test.animals;

/**
 * Klasa narzędziowa App.
 */
public final class App {

    // Prywatny konstruktor, aby uczynić klasę narzędziową.
    private App() {
        throw new UnsupportedOperationException("Utility class");
    }

    /**
     * Główna metoda aplikacji.
     *
     * @param args Argumenty wiersza poleceń.
     */
    public static void main(final String[] args) {
        Animal dog = new Dog("Buddy");
        Animal cat = new Cat("Whiskers");

        dog.makeSound();
        cat.makeSound();
    }
}
