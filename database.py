from threading import Thread
from pymongo import MongoClient
from queue import Queue
import time

class Mongo:
    def __init__(self) -> None:
        self.client = MongoClient(username="raspberrypi", password="123456")
        self.collection = self.client['robot']['system_status']
        document = self.collection.find_one()
        if document is None:
            self.collection.insert_one({'name': 'system_status'})
        self.update_queue = Queue()
        self.t_update()
    
    
    def get_document(self):
        document = self.collection.find_one()
        return document
    
    
    def update_document(self, document):
        self.update_queue.put(document)
    
    
    def _update(self):
        while True:
            if not self.update_queue.empty():
                doc = self.update_queue.get()
                self.collection.replace_one({"name": 'system_status'}, doc)
            else:
                time.sleep(0.5)
    
                
    def t_update(self):
        Thread(target=self._update).start()
                
    
    def __del__(self):
        self.client.close()
        pass