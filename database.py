from pymongo import MongoClient


class Mongo:
    def __init__(self) -> None:
        self.client = MongoClient(username="raspberrypi", password="123456")
        self.collection = self.client['robot']['system_status']
        document = self.collection.find_one()
        if document is None:
            self.collection.insert_one({'name': 'system_status'})
        pass
    
    
    def get_document(self):
        document = self.collection.find_one()
        return document
    
    
    def update_document(self, document):
        self.collection.replace_one({"name": 'system_status'}, document)
        pass
                
    
    def __del__(self):
        self.client.close()
        pass