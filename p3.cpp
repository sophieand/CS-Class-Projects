#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

Info::Info() {
    this->name = "No Name Set";
    this->value = "No Value Set";
    this->next = nullptr;
}

Info::Info(std::string name, std::string value, Info *next) {
    this->name = name;
    this->value = value;
    this->next = next;
}

Contact::Contact() {
    this->first = "No First Set";
    this->last = "No Last Set";
    this->next = nullptr;
    this->headInfoList = nullptr;
}

Contact::Contact(std::string first, std::string last, Contact *next) {
    this->first = first;
    this->last = last;
    this->next = next;
    this->headInfoList = nullptr;
}

Contact::~Contact() {
    Info *temp;
    while (this->headInfoList != nullptr) {
        temp = this->headInfoList->next;
        delete this->headInfoList;
        this->headInfoList = temp;
    }
}

ContactList::ContactList() {
    this->headContactList = nullptr;
    this->count = 0;
}

int ContactList::getCount() {
    return this->count;
}

// print the specified contact and its information
// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(std::ostream &os, std::string first, std::string last) {
    //if contact is not on list
    Contact *cur;
    for (cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first && cur->last == last) {
            break;
        }
    }
    if (cur == nullptr) {
        return false;
    } 
    //print the contact
    for (Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first) {
            if (cur->last == last) {
                os << "Contact Name: " << cur->first << " " << cur->last << endl;
                for (Info *ptr = cur->headInfoList; ptr != nullptr; ptr = ptr->next) {
                    os << "\t" << ptr->name << " | " << ptr->value << endl;
                }
                return true;
            }
        }
    }
    return false;
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(std::ostream &os) {
    if (headContactList == nullptr) {
        return;
    }
    Contact *cur = headContactList;
    while (cur != nullptr) {
        os << "Contact Name: " << cur->first << " " << cur->last << endl;
        if (cur->headInfoList != nullptr) {
            for (Info *ptr = cur->headInfoList; ptr != nullptr; ptr = ptr->next) {
                os << "\t" << ptr->name << " | " << ptr->value << endl;
            }
        }
        cur = cur->next;
    }
}

// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(std::string first, std::string last) {
    // if contact is in list already
    for (Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first) {
            if (cur->last == last) {
                return false;
            }
        }
    }
    Contact *n = new Contact(first, last, nullptr);
    if (headContactList == nullptr) {
        headContactList = n;
        headContactList->next = nullptr;
        count ++;
        return true;
    }
    else {
        Contact *cur = headContactList;
        while (cur->next != nullptr) { //iterating until the end of the list
            cur = cur->next;
        }
        cur->next = n;
        count++;
        return true;
    }
    return false;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(std::string first, std::string last, std::string infoName, std::string infoVal) {
    //if contact is not in list
    Contact *cur;
    for (cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first && cur->last == last) {
            break;
        }
    }
    if (cur == nullptr) {
        return false;
    }
    //if infoname if already in info list, update the infovalue
    for (Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
        if (first == cur->first && last == cur->last) {
            for (Info *ptr = cur->headInfoList; ptr != nullptr; ptr = ptr->next) {
                if (ptr->name == infoName) {
                    ptr->value = infoVal;
                    return true;
                }
            }  
            Info *n = new Info(infoName, infoVal, nullptr);
            if (cur->headInfoList == nullptr) {
                cur->headInfoList = n;
                return true;
            }
            for (Info *ptr = cur->headInfoList; ptr != nullptr; ptr = ptr->next) {
                if (ptr->next == nullptr) {
                    ptr->next = n;
                    return true;
                }
            }
        }
    }
    return false;
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - a compare method/function is recommended
bool ContactList::addContactOrdered(std::string first, std::string last) {
    for (Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first) {
            if (cur->last == last) {
                return false;
            }
        }
    }
    Contact *n = new Contact(first, last);
    if (headContactList == nullptr) { //if list is empty
        headContactList = n;
        count++;
        return true;
    }
    else if (last < headContactList->last) { //if last is smaller than head
        Contact *temp = headContactList;
        headContactList = n;
        n->next = temp;
        count++;
        return true;
    }
    else if (last == headContactList->last) { //if last is same as head
        if (first < headContactList->first) { //first->head->next
            Contact *temp = headContactList;
            headContactList = n;
            n->next = temp;
            count++;
            return true;
        }
    }
    if(headContactList->next == nullptr) {
        headContactList->next = n;
        count++;
        return true;
    }
    for (Contact *cur = headContactList; cur->next != nullptr; cur = cur->next) {
        if (last < cur->next->last) {
            Contact *temp = cur->next;
            cur->next = n;
            n->next = temp;
            count++;
            return true;
        }
        else if (last == cur->next->last) {  //last name == last name
            if (first < cur->next->first) {
                Contact *temp = cur->next;
                cur->next = n;
                n->next = temp;
                count++;
                return true;
            }
        }
        if(cur->next->next == nullptr) {
            cur->next->next = n;
            count++;
            return true;
        }  
    }
    return false;
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(std::string first, std::string last, std::string infoName, std::string infoVal) {
    //if contact is not in list
    Contact *cur;
    for (cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first && cur->last == last) {
            break;
        }
    }
    if (cur == nullptr) {
        return false;
    }
    
    //if infoname if already in info list, update the infovalue
    for (Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
        if (first == cur->first && last == cur->last) {
            for (Info *ptr = cur->headInfoList; ptr != nullptr; ptr = ptr->next) {
                if (ptr->name == infoName) {
                    ptr->value = infoVal;
                    return true;
                }
            }
            Info *n = new Info(infoName, infoVal, nullptr);
            if (cur->headInfoList == nullptr) { //if list is empty
                cur->headInfoList = n;
                return true;
            }
            else if (infoName < cur->headInfoList->name) {
                Info *temp = cur->headInfoList;
                cur->headInfoList = n;
                n->next = temp;
                return true;
            }
            Info *ptr;
            for (ptr = cur->headInfoList; ptr->next != nullptr; ptr = ptr->next) {
                if (infoName < ptr->next->name) {
                    Info *temp = ptr->next;
                    ptr->next = n;
                    n->next = temp;
                    return true;
                }
            }
            if (ptr->next == nullptr) {
                ptr->next = n;
                return true;
            }
        }
    }
    return false;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(std::string first, std::string last) {
    // delete info list
    //change previous to point to the next node after the one removed
    if (!count) {
        return false;
    }

    Contact *prev = headContactList;
    if (headContactList->first == first && headContactList->last == last){
        headContactList = headContactList->next;
        delete prev;
        count--;
        return true;
    }
    for (; prev->next != nullptr; prev = prev->next) {
        if (prev->next->first == first && prev->next->last == last){
            break;
        }
    }
    if (prev->next == nullptr) {
        return false;
    }
    Contact* temp = prev->next;
    prev->next = temp->next;
    delete temp;

    
   /* for (Contact *prev = headContactList; cur != nullptr; prev = prev->next) {
        if (cur->first == first) {
            // if (cur->last == last) {
            //     while (cur->headInfoList != nullptr) {
            //         Info *del = head;
            //         head = head->next;
            //         delete del;
            //         //set equal to null
            //     //cur->headInfoList
            //     delete cur;// TODO: recursion to remove the info??
            // }
        }
    }*/

    count--;
    return true;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(std::string first, std::string last, std::string infoName) {
    // delete info list
    //change previous to point to the next node after the one removed
    if (!count) {
        return false;
    }
    Contact *cur;
    for (cur = headContactList; cur != nullptr; cur = cur->next) {
        if (cur->first == first && cur->last == last) {
            break;
        }
    }
    if (cur == nullptr) {
        return false;
    }
    //if not in list
    for (Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
        if (first == cur->first && last == cur->last) {}
            if (cur->headInfoList == nullptr) {
                return false;
            }
            if (cur->headInfoList->name == infoName) {
                Info* del = cur->headInfoList;
                cur->headInfoList = cur->headInfoList->next;
                delete del;
                return true;
            }
            for (Info *ptr = cur->headInfoList; ptr->next != nullptr; ptr = ptr->next) {
                if (ptr->next->name == infoName) {
                    Info* temp = ptr->next->next;
                    delete ptr->next;
                    ptr->next = temp;
                    return true;
                }
            } 
    }

    return false;
}

// destroy the list by removing all contacts and their infos
// PROBLEM: INFO NEEDS TO BE SEPERATED DELETED??
ContactList::~ContactList() {
    while (headContactList != nullptr) {
        //iterate to the BACK of the contact list
        //delete each info list
        //set each node equal to nullptr
        Contact *del = headContactList;
        headContactList = headContactList->next;
        delete del;
    }
}

// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList &src) {
    headContactList = nullptr;
    count = 0;
    for (Contact *cur = src.headContactList; cur != nullptr; cur = cur->next) {
        addContact(cur->first, cur->last);
        for (Info *ptr = cur->headInfoList; ptr != nullptr; ptr = ptr->next) {
            addInfo(cur->first, cur->last, ptr->name, ptr->value);
        }
        count++;
    }
}
 
// remove all contacts and their info then deep copy the source list
// - do not forget to update count
const ContactList &ContactList::operator=(const ContactList &src) {
    while (headContactList != nullptr) {
        //iterate to the BACK of the contact list
        //delete each info list
        //set each node equal to nullptr
        Contact *del = headContactList;
        headContactList = headContactList->next;
        delete del;
    }
    for (Contact *cur = src.headContactList; cur != nullptr; cur = cur->next) {
        addContact(cur->first, cur->last);
        for (Info *ptr = cur->headInfoList; ptr != nullptr; ptr = ptr->next) {
            addInfo(cur->first, cur->last, ptr->name, ptr->value);
        }
        //count++;
    }
    count = src.count;
    return *this;
}
