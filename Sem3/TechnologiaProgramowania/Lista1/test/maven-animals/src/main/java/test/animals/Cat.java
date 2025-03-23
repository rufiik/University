package test.animals;
/**
 * Klasa reprezentująca kota.
 */
public final class Cat extends Animal {
/**
 * Konstruktor klasy Cat.
 * @param name - nazwa
 */
    public Cat(final String name) {
        super(name);
    }

     /**
     *  Wydaje dźwięk kota.
     */
    @Override
    public void makeSound() {
        System.out.println(getName() + " says Meow!");
    }
}
