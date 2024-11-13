import org.springframework.beans.BeanUtils;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.TimeUnit;

@SpringBootApplication
public class CopyDemoApplication {
    public static void main(String[] args) {
        User user = new User();
        user.setId("1");
        user.setName("zhangsan");
        user.setAge(30);
        user.setBirthDate(new Date());
        User.Address address = new User.Address();
        address.setStreet("hz");
        address.setCity("hz");
        address.setZipCode("12345");
        address.setCountry("USA");
        user.setAddress(address);
        user.getHobbies().add("Reading");
        user.getHobbies().add("Hiking");
        user.getPhones().add(new User.Phone("123-456-7890", "mobile"));
        user.getPhones().add(new User.Phone("987-654-3210", "home"));
        User.EmergencyContact emergencyContact = new User.EmergencyContact();
        emergencyContact.setName("lisi");
        emergencyContact.setRelationship("sssssss");
        emergencyContact.setPhoneNumber("2222222222");
        user.setEmergencyContact(emergencyContact);
        long startTime = System.nanoTime();
        for (int i = 0; i < 10000000; i++) {
            UserDTO userDTO = new UserDTO();
            BeanUtils.copyProperties(userDTO, user);
        }
        long endTime = System.nanoTime();
        long duration = TimeUnit.NANOSECONDS.toMillis(endTime - startTime);
        System.out.println("Copying 10,000,000 User objects to UserDTO objects took: " + duration + " ms");
    }
}

class User {
    private String id;
    private String name;
    private int age;
    private Date birthDate;
    private Address address;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public Date getBirthDate() {
        return birthDate;
    }

    public void setBirthDate(Date birthDate) {
        this.birthDate = birthDate;
    }

    public Address getAddress() {
        return address;
    }

    public void setAddress(Address address) {
        this.address = address;
    }

    public List<String> getHobbies() {
        return hobbies;
    }

    public void setHobbies(List<String> hobbies) {
        this.hobbies = hobbies;
    }

    public Set<Phone> getPhones() {
        return phones;
    }

    public void setPhones(Set<Phone> phones) {
        this.phones = phones;
    }

    public EmergencyContact getEmergencyContact() {
        return emergencyContact;
    }

    public void setEmergencyContact(EmergencyContact emergencyContact) {
        this.emergencyContact = emergencyContact;
    }

    private List<String> hobbies;
    private Set<Phone> phones;
    private EmergencyContact emergencyContact;

    public User() {
        hobbies = new ArrayList<>();
        phones = new HashSet<>();
    }

    public static class Address {
        private String street;
        private String city;
        private String zipCode;

        public String getStreet() {
            return street;
        }

        public void setStreet(String street) {
            this.street = street;
        }

        public String getCity() {
            return city;
        }

        public void setCity(String city) {
            this.city = city;
        }

        public String getZipCode() {
            return zipCode;
        }

        public void setZipCode(String zipCode) {
            this.zipCode = zipCode;
        }

        public String getCountry() {
            return country;
        }

        public void setCountry(String country) {
            this.country = country;
        }

        private String country;

    }

    public static class Phone {
        private String number;
        private String type;

        public Phone(String s, String mobile) {
            this.number = s;
            this.type = mobile;
        }

        public String getNumber() {
            return number;
        }

        public void setNumber(String number) {
            this.number = number;
        }

        public String getType() {
            return type;
        }

        public void setType(String type) {
            this.type = type;
        }
    }

    public static class EmergencyContact {
        private String name;
        private String relationship;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getRelationship() {
            return relationship;
        }

        public void setRelationship(String relationship) {
            this.relationship = relationship;
        }

        public String getPhoneNumber() {
            return phoneNumber;
        }

        public void setPhoneNumber(String phoneNumber) {
            this.phoneNumber = phoneNumber;
        }

        private String phoneNumber;
    }
}

class UserDTO {
    private String userId;
    private String username;
    private int userAge;
    private Date birthDate;

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public int getUserAge() {
        return userAge;
    }

    public void setUserAge(int userAge) {
        this.userAge = userAge;
    }

    public Date getBirthDate() {
        return birthDate;
    }

    public void setBirthDate(Date birthDate) {
        this.birthDate = birthDate;
    }

    public String getAddressInfo() {
        return addressInfo;
    }

    public void setAddressInfo(String addressInfo) {
        this.addressInfo = addressInfo;
    }

    public List<String> getUserHobbies() {
        return userHobbies;
    }

    public void setUserHobbies(List<String> userHobbies) {
        this.userHobbies = userHobbies;
    }

    public List<String> getPhoneNumbers() {
        return phoneNumbers;
    }

    public void setPhoneNumbers(List<String> phoneNumbers) {
        this.phoneNumbers = phoneNumbers;
    }

    public String getEmergencyContactInfo() {
        return emergencyContactInfo;
    }

    public void setEmergencyContactInfo(String emergencyContactInfo) {
        this.emergencyContactInfo = emergencyContactInfo;
    }

    private String addressInfo;
    private List<String> userHobbies;
    private List<String> phoneNumbers;
    private String emergencyContactInfo;

    public UserDTO() {
        userHobbies = new ArrayList<>();
        phoneNumbers = new ArrayList<>();
    }
}
