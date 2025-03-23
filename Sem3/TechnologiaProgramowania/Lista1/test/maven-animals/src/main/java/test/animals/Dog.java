package test.animals;

/**
 * Klasa reprezentująca psa.
 */
public class Dog extends Animal {

    /**
     * Konstruktor klasy Dog.
     *
     * @param name Imię psa.
     */
    public Dog(final String name) {
        super(name);
    }

    /**
     * Wydaje dźwięk psa.
     */
    @Override
    public void makeSound() {
        System.out.println(getName() + " says Woof!");
    }
}
