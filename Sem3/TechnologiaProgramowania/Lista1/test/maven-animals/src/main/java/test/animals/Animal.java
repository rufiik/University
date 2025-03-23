package test.animals;
/**
 * Klasa reprezentująca zwierzę.
 */
public class Animal {
    /**
     * Nazwa zwierzęcia.
     */
    private final String name;
    /**
     * Konstruktor klasy Animal.
     * @param nameParam - nazwa
     */
    public  Animal(final String nameParam) {
        this.name = nameParam;
    }
/**
 * Metoda getName.
 * @return name - zwraca nazwe
 */
    public final String getName() {
        return name;
    }
/**
 * Metoda makeSound.
 */
    public void makeSound() {
        System.out.println("Animal makes a sound");
    }
}
