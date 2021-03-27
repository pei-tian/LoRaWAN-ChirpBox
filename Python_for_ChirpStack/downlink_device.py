import os
import sys

import grpc
from chirpstack_api.as_pb.external import api
import time

# Configuration.

# This must point to the API interface.
# server = "192.168.137.37:8080"
# server = "3m364v9352.imdo.co:31566"
server = "192.168.137.228:8080"

# The DevEUI for which you want to enqueue the downlink.
dev_eui = bytes([0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x34])

# The API token (retrieved using the web-interface).
api_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJjaGlycHN0YWNrLWFwcGxpY2F0aW9uLXNlcnZlciIsImF1ZCI6ImNoaXJwc3RhY2stYXBwbGljYXRpb24tc2VydmVyIiwic3ViIjoidXNlciIsInVzZXJuYW1lIjoiYWRtaW4ifQ.jQjVFrYYj9wbGmI7QKb90INACuZ8Po-jHAcm9-C1eVM"

# if __name__ == "__main__":
def group_send_on_packet():
  # Connect without using TLS.
  channel = grpc.insecure_channel(server)

  # Device-queue API client.
  client = api.DeviceQueueServiceStub(channel)

  # Define the API key meta-data.
  auth_token = [("authorization", "Bearer %s" % api_token)]

  # Construct request.
  req = api.EnqueueDeviceQueueItemRequest()
  req.device_queue_item.confirmed = False
  req.device_queue_item.data = bytes([0x01, 0x02, 0x03])
  req.device_queue_item.dev_eui = dev_eui.hex()
  req.device_queue_item.f_port = 10
  print(req)

  resp = client.Enqueue(req, metadata=auth_token)

  # Print the downlink frame-counter value.
  print(resp.f_cnt)


def lorawan_class_dl(num_packet):
  for i in range(num_packet):
    print(i)
    group_send_on_packet()
    time.sleep(10)

lorawan_class_dl(240)