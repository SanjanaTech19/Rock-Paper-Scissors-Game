"""
# class variables and instance variables
# class variables can be accessed by all the instances of the class
# class variables are used to share data amongst all instances of a class
# class variables are declared oputside the constructor
class Student:
    class_year = 2028 # class variable
    num_students = 0
    
    
    def __init__(self,name,age):
        self.name = name
        self.age = age
        Student.num_students += 1

student1 = Student("sanjana",20)
student2 = Student("sally",20)


print(student1.name,student1.age)
print(student2.name,student2.age)
print(Student.num_students)
print(f"My graduating class of {Student.class_year} has {Student.num_students} students")


"""

# inheritance - allows a class to inherit the attributes and methods fromk another class
# it helps with code reusability and extensibility
# parent class and child class - sub and super classes

"""
class Animal:
    def __init__(self,name):
        self.name = name
        self.isalive = True

    def eat(self):
        print(f"{self.name} is eating") 

    def sleep(self):
        print(f"{self.name} is sleeping")


class Dog(Animal):
    def speak(self):
        print("woof")

class Cat(Animal):
    def speak(self):
        print("meow")

class Mouse(Animal):
    def speak(self):
        print("squeak")

dog = Dog("saba")
cat = Cat("garfield")
mouse = Mouse("stuart")

print(mouse.name)
print(mouse.isalive)
mouse.eat()
mouse.sleep()
dog.speak()

"""


# multiple inheritance - child class inherits from more than one parent class C(A,B)

# multilevel inheritance - a parent can inherit from another parent C(B) -> B(A) -> A

"""
class Animal:

    def __init__(self,name):
        self.name = name
    def eat(self):
        print(f"{self.name}is eating")

    def sleep(self):
        print(f"{self.name} is sleeping")

class Prey(Animal):
    def flee(self):
        print(f"{self.name} is fleeing")

class Predator(Animal):
    def hunt(self):
        print(f"{self.name} is hunting")

class Rabbit(Prey):
    pass

class Hawk(Predator):
    pass

class Fish(Prey,Predator):
    pass

rabbit = Rabbit("bobby")
hawk = Hawk("tommy")
fish = Fish("sweety")

rabbit.flee()

"""

"""
class Employees:
    def __init__(self,name):
        self.name = name

    def working(self):
        print(f"{self.name} is working")

    def notworking(self):
        print(f"{self.name} is not working")


class Tech(Employees):
    def automation(self):
        print(f"{self.name} is working in tech")

    def onleave(self):
        print(f"{self.name} is on leave")


class Finance(Employees):
    def accounts(self):
        print(f"{self.name} is working on accounts")

    def onleave(self):
        print(f"{self.name} is on leave")

class Software(Tech):
    def development(self):
        print(f"{self.name} is a developer")

class Fintech(Finance):
    def assessment(self):
        print(f"{self.name} is a financial analyst")


emp1 = Software("bobby")
emp2 = Fintech("sunny")

emp1.development()

"""


# abstract class - cannot be instantiated on its own .. meant to be subclassed
# can contain abstract methods----> declared but have no implementation
# prevents instantiation of the class itself
# requires children to use inherited abstract methods
# all abstract methods inside the abstract class msut be used in the child classes

"""
from abc import ABC,abstractmethod

class Vehicle(ABC):
    
    @abstractmethod
    def go(self):
        pass

    @abstractmethod
    def stop(self):
        pass

class Car(Vehicle):
    def go(self):
        print("you drive the car")

    def stop(self):
        print("you stop the car")

class Motorcycle(Vehicle):
    def go(self):
        print("you ride the motorcycle")

    def stop(self):
        print("you stop the motorcycle")

class Boat(Vehicle):
    def go(self):
        print("you sail the boat")
    
    def stop(self):
        print("you anchor the boat")


boat = Boat()
boat.go()
boat.stop()
"""


#super() - function used in child class to call methods from a parent class
# allows you to extend functionality

"""
class Shape:
    def __init__(self,colour,isfilled):
        self.colour = colour
        self.isfilled = isfilled

    def describe(self):
        print(f"it is {self.colour} and {'filled' if self.isfilled else 'not filled'}")

class Circle(Shape):
    def __init__(self,colour,isfilled,radius):
        super().__init__(colour,isfilled)
        self.radius = radius

    def Area(self):
        print(f"The area of the circle is {3.14 * self.radius * self.radius}")

class Square(Shape):
    def __init__(self,colour,isfilled,width):
        super().__init__(colour,isfilled)
        self.width = width

    def Area(self):
        print(f"The area of the sqaure is {self.width * self.width}")

class Triangle(Shape):
    def __init__(self,colour,isfilled,width,height):
        super().__init__(colour,isfilled)
        self.width = width
        self.height = height

    def Area(self):
        print(f"The area of the triangle is {0.5 * self.width *self.height}")

circle = Circle(colour="red", isfilled=True, radius=5)
print(circle.colour)
print(circle.isfilled)
print(circle.radius)
circle.describe()
circle.Area()

square = Square(colour="blue", isfilled=False, width=4)
print(square.colour)
print(square.isfilled)
print(square.width)
square.describe()
square.Area()

triangle = Triangle(colour="green", isfilled=True, width=10, height=10)
print(triangle.colour)
print(triangle.isfilled)
print(triangle.width)
print(triangle.height)
triangle.describe()
triangle.Area()

"""

# polymorphism - to have many forms
# can be achieved through inheritance and duck typing 
# duck typing - object must have necessary attributes and methods

"""
from abc import ABC , abstractmethod

class Shape:
    @abstractmethod
    def area(self):
        pass


class Circle(Shape):
    def __init__(self,radius):
        self.radius = radius

    def area(self):
        return 3.14 * self.radius * self.radius
    
class Square(Shape):
    def __init__(self,side):
        self.side = side

    def area(self):
        return self.side * self.side

class Triangle(Shape):
    def __init__(self,base,height):
        self.base = base
        self.height = height

    def area(self):
        return 0.5 * self.base * self.height

shapes = [Circle(4),Square(5),Triangle(6,7)]

for shape in shapes:
    print(shape.area())

"""

# duck typing - object must have the necessary attributes and methods
#  if it looks like a duck , quacks like a duck then it must be a duck

"""
class Animal:
    alive = True

class Dog(Animal):
    def speak(self):
        print("woof")

class Cat(Animal):
    def speak(self):
        print("meow")

animals = [Dog(),Cat()]

for animal in animals:
    animal.speak()
    print(animal.alive)

"""

# aggregation - represents a relationship where one object contains references to one or more independent objects

"""
class Library:
    def __init__(self,name):
        self.name = name
        self.books = []
        
    def addbooks(self,book):
        self.books.append(book)

    def listbooks(self):
        return [f"{book.title} by {book.author}" for book in self.books]
class Book:
    def __init__(self,title,author):
        self.title = title
        self.author = author

library = Library("Sydney central Library")

book1 = Book("Harry Potter","JK rowling")
book2 = Book("Hobbit","JRR tolken")
book3 = Book("the colour of magic","terry pratches")

library.addbooks(book1)
library.addbooks(book2)
library.addbooks(book3)

print(library.name)
print(library.listbooks())

"""

# composition - the composed object directly owns its components which cannot exist independently owns a relationship

"""
class Engine:
    def __init__(self,horsepower):
        self.horsepower = horsepower

class Wheel:
    def __init__(self,size):
        self.size = size

class Car:
    def __init__(self,make,model,horsepower,wheelsize):
        self.make = make
        self.model = model
        self.engine = Engine(horsepower)
        self.wheels = [Wheel(wheelsize) for wheel in range(4)]

    def displaycar(self):
        return f"{self.make} {self.model} {self.engine.horsepower} {self.wheels[0].size}"

car = Car(make="ford",model="mustang",horsepower=500,wheelsize=18)

print(car.displaycar())

"""


# Nested class - class within another class
# encapsulates private details that arent relevant outside the outer class
# reduces possibility of naming conflicts

"""
class Company:
    class Employee:
        def __init__(self,name,position):
            self.name = name
            self.position = position

        def getdetails(self):
            return f"{self.name} {self.position}"

    def __init__(self,companyname):
        self.companyname = companyname
        self.employees = []

    def addemployee(self,name,position):
        newemployee = self.Employee(name,position)
        self.employees.append(newemployee)

    def listemployee(self):
        return [employee.getdetails for employee in self.employees]

company = Company("krusty crab")

print(company.companyname)
company.addemployee("sam","manager")
company.addemployee("sal","supervisor")

print(company.listemloyee)

"""

# Static Methods - a methods that belongs to a class rather than any object from that class.. best for utility functions that do not need access to class data
# instance methods - best for operations on instances of the class 

"""
class Employee:
    def __init__(self,name,position):
        self.name = name
        self.position = position

    def getinfo(self):
        return f"{self.name} {self.position}"
    
    @staticmethod
    def isvalidposition(position):
        validpositions = ["manager","cashier","cook","janitor"]
        return position in validpositions 

emp1 = Employee("eugene","manager")
print(emp1.getinfo())
print(Employee.isvalidposition("cook"))

"""

# class methods - allow operations related to the class itself
# take cls as the first parameter which represents the class itself

"""
class Student:
    
    count = 0
    totalgpa = 0
    
    def __init__(self,name,gpa):
        self.name = name
        self.gpa = gpa
        Student.count += 1
        Student.totalgpa += gpa

    def getinfo(self):
        return f"{self.name} {self.gpa}"
    
    @classmethod
    def getcount(cls):
        return f"total number of students {cls.count}"
    
    @classmethod
    def getaveragegpa(cls):
        if cls.count == 0:
            return 0
        else:
            return f"{cls.totalgpa / cls.count}"
    
student1 = Student("spongebob",3.2)
student2 = Student("sandy",4.0)

print(Student.getcount())
print(Student.getaveragegpa())

"""

# Magic methods - dunder methods __init__ , __str__ , __eq__
# they are automatically called by many of pythons built in operations
# they allow developers to define or customize the behavious of objects 

